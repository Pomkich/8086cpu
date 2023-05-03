#pragma once
#include "Constants.h"
#include "AbstractPresenter.h"
#include "cpu8086.h"
#include "Memory.h"
#include <list>
#include <string>

// ������� ��������� ������������, 
// ���������: 
// ��������� �� ���������, 
// ��������� �� ������, 
// ��������� �� ����������, ������� ������� � ����������� ������ ���������� 
// ��������, ������� �����������, ���� ������ � ��������
// ��������������, ��� �� ������ ������ ������� ��������� �������� � ��������
void GenerateLab(
	std::shared_ptr<cpu8086> cpu_pt,
	std::shared_ptr<Memory> mem_pt, 
	std::shared_ptr<bool> running,
	std::list<RegId> regs, int mem_start, int mem_end, std::string deskription);


void VerifyLab(std::string lab_file_name) {};