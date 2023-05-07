#pragma once
#include "Constants.h"
#include "AbstractPresenter.h"
#include "cpu8086.h"
#include "Memory.h"
#include <list>
#include <string>
#include <vector>
#include <boost/json.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

// функци€ генерации лабораторных, 
// аргументы: 
// указатель на процессор, 
// указатель на пам€ть, 
// указатель на переменную, котора€ говорит о прекращении работы процессора 
// регистры, которые провер€ютс€, блок пам€ти, название лабораторной и описание
// предполагаетс€, что на момент вызова функции программа загружен в эмул€тор
void GenerateLab(
	std::shared_ptr<cpu8086> cpu_pt,
	std::shared_ptr<Memory> mem_pt, 
	std::shared_ptr<bool> running,
	std::list<RegId> regs, int mem_start, int mem_end, 
	std::string lab_name, std::string description
);


bool VerifyLab(
	std::shared_ptr<cpu8086> cpu_pt,
	std::shared_ptr<Memory> mem_pt,
	std::shared_ptr<bool> running,
	std::string lab_file_name
);

std::string GetDescription(std::string file_name);