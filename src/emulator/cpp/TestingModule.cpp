#include "TestingModule.h"

void GenerateLab(
	std::shared_ptr<cpu8086> cpu_pt,
	std::shared_ptr<Memory> mem_pt,
	std::shared_ptr<bool> running, 
	std::list<RegId> regs, int mem_start, int mem_end, 
	std::string lab_name, std::string description) {
	// записать описание в файл
	// создать тест:
	//		сгенерировать входные данные для регистров и памяти
	//		записать входные данные
	//		запустить выполнение программы
	//		прочитать и записать выходные данные
	// создать ещё один текст

	boost::json::object out_data;
	out_data["Description"] = description;
	// запись адреса начала блока памяти
	out_data["StartAddress"] = mem_start;

	word cs_copy = cpu_pt->getRegVal(RegId::CS);
	word ip_copy = cpu_pt->getRegVal(RegId::IP);
	word ds_copy = cpu_pt->getRegVal(RegId::DS);
	word ss_copy = cpu_pt->getRegVal(RegId::SS);
	word sp_copy = cpu_pt->getRegVal(RegId::SP);
	word es_copy = cpu_pt->getRegVal(RegId::ES);

	for (int test = 0; test < 10; test++) {
		// восстанавление начального значения сегментов
		cpu_pt->setRegVal(RegId::CS, cs_copy);
		cpu_pt->setRegVal(RegId::IP, ip_copy);
		cpu_pt->setRegVal(RegId::DS, ds_copy);
		cpu_pt->setRegVal(RegId::SS, ss_copy);
		cpu_pt->setRegVal(RegId::SP, sp_copy);
		cpu_pt->setRegVal(RegId::ES, es_copy);

		std::string test_N = "test_" + std::to_string(test);
		out_data[test_N].emplace_object();

		// восстанавливаение регистров сегментов (пока только кода)
		cpu_pt->setRegVal(RegId::CS, cs_copy);
		cpu_pt->setRegVal(RegId::IP, ip_copy);
		// генерация случайных данных в регистры
		dword value = 0;
		for (auto id : regs) {
			value = rand() % 65000;	// ~2^16
			cpu_pt->setRegVal(id, value);
			out_data[test_N].get_object()[std::to_string((int)id)] = { {"in", value}, {"out", 0} };
		}
		// генерация случайных данных в память
		out_data[test_N].get_object()["MemDumpIN"].emplace_array();	// инициализируем массив	
		for (int address = mem_start; address < mem_end; address++) {
			value = rand() % 255;
			mem_pt->writeB(address, value);
			out_data[test_N].get_object()["MemDumpIN"].get_array().emplace_back(value);
		}

		// запускаем программу
		*running = true;
		while (*running) {
			cpu_pt->clock();
		}

		// чтение выходных значений регистров
		for (auto id : regs) {
			value = cpu_pt->getRegVal(id);
			out_data[test_N].get_object()[std::to_string((int)id)].get_object()["out"] = value;
		}
		// чтение выходных значений памяти
		out_data[test_N].get_object()["MemDumpOUT"].emplace_array();
		for (int address = mem_start; address < mem_end; address++) {
			value = mem_pt->readB(address);
			out_data[test_N].get_object()["MemDumpOUT"].get_array().emplace_back(value);
		}
	}
	// запись в файл структуры данных
	std::ofstream file(lab_name + ".json");
	file << boost::json::serialize(out_data);
	file.close();
}

bool VerifyLab(
	std::shared_ptr<cpu8086> cpu_pt, 
	std::shared_ptr<Memory> mem_pt,
	std::shared_ptr<bool> running,
	std::string lab_file_name) {
	// загрузить данные
	// запустить тест
	//		запустить программу
	//		проверить соответствие данных
	//		если не соответствуют -> return false
	// запустить следующий тест
	// return true

	// загрузка данных в root
	boost::property_tree::ptree root;
	boost::property_tree::read_json(lab_file_name, root);
	// адрес начала блока памяти
	int start_address = root.get<int>("StartAddress");
	// начальные значения сегментов
	word cs_copy = cpu_pt->getRegVal(RegId::CS);
	word ip_copy = cpu_pt->getRegVal(RegId::IP);
	word ds_copy = cpu_pt->getRegVal(RegId::DS);
	word ss_copy = cpu_pt->getRegVal(RegId::SS);
	word sp_copy = cpu_pt->getRegVal(RegId::SP);
	word es_copy = cpu_pt->getRegVal(RegId::ES);

	for (int test = 0; test < 10; test++) {
		// восстанавление начального значения сегментов
		cpu_pt->setRegVal(RegId::CS, cs_copy);
		cpu_pt->setRegVal(RegId::IP, ip_copy);
		cpu_pt->setRegVal(RegId::DS, ds_copy);
		cpu_pt->setRegVal(RegId::SS, ss_copy);
		cpu_pt->setRegVal(RegId::SP, sp_copy);
		cpu_pt->setRegVal(RegId::ES, es_copy);

		// заполнение входными данными регистров
		std::string test_N = "test_" + std::to_string(test);
		for (const auto& node : root.get_child(test_N)) {
			if (node.first.size() <= 2) {
				int value = node.second.get<int>("in");
				int id = std::stoi(node.first);
				cpu_pt->setRegVal((RegId)id, value);
			}
		}
		// заполнение входными данными памяти
		int offset = 0;
		for (const auto& node : root.get_child(test_N + ".MemDumpIN")) {
			mem_pt->writeB(start_address + offset, node.second.get_value<int>());
			offset++;
		}

		// запуск программы
		*running = true;
		while (*running) {
			cpu_pt->clock();
		}

		// проверка регистров на соответствие
		for (const auto& node : root.get_child(test_N)) {
			if (node.first.size() <= 2) {
				int value = node.second.get<int>("out");
				int id = std::stoi(node.first);
				if (cpu_pt->getRegVal((RegId)id) != value) {
					return false; // значение не совпало -> лабораторная не выполнена
				}
			}
		}
		// проверка памяти на соответствие
		offset = 0;
		for (const auto& node : root.get_child(test_N + ".MemDumpOUT")) {
			int value = mem_pt->readB(start_address + offset);
			if (value != node.second.get_value<int>()) {
				return false;
			}
			offset++;
		}
	}

	return true;
}

std::string GetDescription(std::string file_name) {
	// загрузка данных в root
	boost::property_tree::ptree root;
	boost::property_tree::read_json(file_name, root);
	return root.get<std::string>("Description");
}