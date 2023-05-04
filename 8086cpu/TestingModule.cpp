#include "TestingModule.h"

void GenerateLab(
	std::shared_ptr<cpu8086> cpu_pt,
	std::shared_ptr<Memory> mem_pt,
	std::shared_ptr<bool> running, 
	std::list<RegId> regs, int mem_start, int mem_end, std::string lab_name, std::string description) {
	// записать описание в файл
	// создать тест:
	//		сгенерировать входные данные для регистров и памяти
	//		записать входные данные
	//		запустить выполнение программы
	//		прочитать и записать выходные данные
	// создать ещё один текст

	boost::json::object out_file;
	out_file["Description"] = description;

	word cs_copy = cpu_pt->getRegVal(RegId::CS);
	word ip_copy = cpu_pt->getRegVal(RegId::IP);

	for (int test = 0; test < 10; test++) {
		std::string test_N = "test_" + std::to_string(test);
		out_file[test_N].emplace_object();

		// восстанавливаем регистры сегментов (пока только кода)
		cpu_pt->setRegVal(RegId::CS, cs_copy);
		cpu_pt->setRegVal(RegId::IP, ip_copy);
		// генерируем случайные данные в регистры
		dword value = 0;
		for (auto id : regs) {
			value = rand() % 65000;	// ~2^16
			cpu_pt->setRegVal(id, value);
			out_file[test_N].get_object()[std::to_string((int)id)] = { {"in", value}, {"out", 0} };
		}
		// генерируем случайные данные в память
		out_file[test_N].get_object()["MemDumpIN"].emplace_array();	// инициализируем массив
		//out_file["MemDumpIN"].emplace_array();	
		for (int address = mem_start; address < mem_end; address++) {
			value = rand() % 255;
			mem_pt->writeB(address, value);
			out_file[test_N].get_object()["MemDumpIN"].get_array().emplace_back(value);
		}

		// запускаем программу
		*running = true;
		while (*running) {
			cpu_pt->clock();
		}

		// читаем выходные значения регистров
		for (auto id : regs) {
			value = cpu_pt->getRegVal(id);
			out_file[test_N].get_object()[std::to_string((int)id)].get_object()["out"] = value;
		}
		// читаем выходные значения памяти
		out_file[test_N].get_object()["MemDumpOUT"].emplace_array();
		for (int address = mem_start; address < mem_end; address++) {
			value = mem_pt->readB(address);
			out_file[test_N].get_object()["MemDumpOUT"].get_array().emplace_back(value);
		}
	}
	// записываем в файл структуру
	std::ofstream file(lab_name + ".json");
	file << boost::json::serialize(out_file);
}

void VerifyLab(std::string lab_file_name) {

}