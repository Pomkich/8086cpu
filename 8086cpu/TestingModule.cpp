#include "TestingModule.h"

void GenerateLab(
	std::shared_ptr<cpu8086> cpu_pt,
	std::shared_ptr<Memory> mem_pt,
	std::shared_ptr<bool> running, 
	std::list<RegId> regs, int mem_start, int mem_end, 
	std::string lab_name, std::string description) {
	// �������� �������� � ����
	// ������� ����:
	//		������������� ������� ������ ��� ��������� � ������
	//		�������� ������� ������
	//		��������� ���������� ���������
	//		��������� � �������� �������� ������
	// ������� ��� ���� �����

	boost::json::object out_data;
	out_data["Description"] = description;

	word cs_copy = cpu_pt->getRegVal(RegId::CS);
	word ip_copy = cpu_pt->getRegVal(RegId::IP);

	for (int test = 0; test < 10; test++) {
		std::string test_N = "test_" + std::to_string(test);
		out_data[test_N].emplace_object();

		// ��������������� �������� ��������� (���� ������ ����)
		cpu_pt->setRegVal(RegId::CS, cs_copy);
		cpu_pt->setRegVal(RegId::IP, ip_copy);
		// ���������� ��������� ������ � ��������
		dword value = 0;
		for (auto id : regs) {
			value = rand() % 65000;	// ~2^16
			cpu_pt->setRegVal(id, value);
			out_data[test_N].get_object()[std::to_string((int)id)] = { {"in", value}, {"out", 0} };
		}
		// ���������� ��������� ������ � ������
		out_data[test_N].get_object()["MemDumpIN"].emplace_array();	// �������������� ������
		//out_file["MemDumpIN"].emplace_array();	
		for (int address = mem_start; address < mem_end; address++) {
			value = rand() % 255;
			mem_pt->writeB(address, value);
			out_data[test_N].get_object()["MemDumpIN"].get_array().emplace_back(value);
		}

		// ��������� ���������
		*running = true;
		while (*running) {
			cpu_pt->clock();
		}

		// ������ �������� �������� ���������
		for (auto id : regs) {
			value = cpu_pt->getRegVal(id);
			out_data[test_N].get_object()[std::to_string((int)id)].get_object()["out"] = value;
		}
		// ������ �������� �������� ������
		out_data[test_N].get_object()["MemDumpOUT"].emplace_array();
		for (int address = mem_start; address < mem_end; address++) {
			value = mem_pt->readB(address);
			out_data[test_N].get_object()["MemDumpOUT"].get_array().emplace_back(value);
		}
	}
	// ���������� � ���� ���������
	std::ofstream file(lab_name + ".json");
	file << boost::json::serialize(out_data);
	file.close();
}

bool VerifyLab(
	std::shared_ptr<cpu8086> cpu_pt, 
	std::shared_ptr<Memory> mem_pt,
	std::shared_ptr<bool> running,
	std::string lab_file_name) {
	// ��������� ������
	// ��������� ����
	//		��������� ���������
	//		��������� ������������ ������
	//		���� �� ������������� -> return false
	// ��������� ��������� ����
	// return true

	// �������� ������ � root
	boost::property_tree::ptree root;
	boost::property_tree::read_json("lab1.json", root);
	// ��������� �������� ���������
	word cs_copy = cpu_pt->getRegVal(RegId::CS);
	word ip_copy = cpu_pt->getRegVal(RegId::IP);

	for (int test = 0; test < 10; test++) {
		cpu_pt->setRegVal(RegId::IP, ip_copy);
		cpu_pt->setRegVal(RegId::CS, cs_copy);

		// ���������� �������� ������� �������� � ������
		std::string test_N = "test_" + std::to_string(test);
		for (const auto& node : root.get_child(test_N)) {
			if (node.first.size() <= 2) {	// ������ � ����������
				int value = node.second.get<int>("in");
				int id = std::stoi(node.first);
				cpu_pt->setRegVal((RegId)id, value);
			}
			else if (node.first == "MemDumpIN") { // ������ � �������

			}
		}
		// ������ ���������
		*running = true;
		while (*running) {
			cpu_pt->clock();
		}

		for (const auto& node : root.get_child(test_N)) {
			if (node.first.size() <= 2) {	// �������� � ����������
				int value = node.second.get<int>("out");
				int id = std::stoi(node.first);
				if (cpu_pt->getRegVal((RegId)id) != value) {
					return false; // �������� �� ������� -> ������������ �� ���������
				}
			}
			else if (node.first == "MemDumpOUT") { // �������� � �������

			}
		}
	}

	return true;
}