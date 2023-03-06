#include "cpu8086.h"

// конструктор
cpu8086::cpu8086() {
	flag_reg = 0;
	IP = 0;
	CS = 0xFFFF;
	DS = 0;
	SS = 0;
	ES = 0;
	SP = 0;
	BP = 0;
	SI = 0;
	DI = 0;
	A.X = 0;
	B.X = 0;
	C.X = 0;
	D.X = 0;

	opcode = 0;
	address = 0;
}

// сброс состояния процессора
void cpu8086::reset() {
	flag_reg = 0;
	IP = 0;
	CS = 0xFFFF;
	DS = 0;
	SS = 0;
	ES = 0;
}

// функция выполнения одной команды
void cpu8086::clock() {
	address = ((dword)CS << 4) + IP;	// генерация физического адреса
	opcode = memory->readB(address);	// получение команды
	opcode_table[opcode]();				// выполнение команды
	IP++;								// установка счётчика на следующую команду
}

void cpu8086::initMemory(std::shared_ptr<Memory> mem) {
	memory = mem;
}

// фкнкции проверки флагов
void cpu8086::testFlagZ(word src_op) {
	(src_op == 0) ? setFlag(Flag::Z) : remFlag(Flag::Z);
}

void cpu8086::testFlagSB(byte src_op) {
	(src_op >> (sizeof(byte) * 8 - 1) & 1) ? setFlag(Flag::S) : remFlag(Flag::S);
}
void cpu8086::testFlagSW(word src_op) {
	(src_op >> (sizeof(word) * 8 - 1) & 1) ? setFlag(Flag::S) : remFlag(Flag::S);
}

void cpu8086::testFlagPB(byte val) {
	word set_bits = 0;
	for (int i = 0; i < sizeof(byte) * 8; i++) {
		if ((val & 1) == 1) set_bits++;
		val = val >> 1;
	}
	if (set_bits % 2 == 0) setFlag(Flag::P);	// parity bit set
	else remFlag(Flag::P);
}

void cpu8086::testFlagPW(word val) {
	word set_bits = 0;
	for (int i = 0; i < sizeof(word) * 8; i++) {
		if ((val & 1) == 1) set_bits++;
		val = val >> 1;
	}
	if (set_bits % 2 == 0) setFlag(Flag::P);	// parity bit set
	else remFlag(Flag::P);
}

void cpu8086::testFlagCAddB(byte prev_val, byte src_op) {
	if (prev_val > src_op) setFlag(Flag::C);	// carry bit set add
	else remFlag(Flag::C);
}

void cpu8086::testFlagCSubB(byte prev_val, byte src_op) {
	if (prev_val < src_op) setFlag(Flag::C);	// carry bit set sub
	else remFlag(Flag::C);
}

void cpu8086::testFlagCAddW(word prev_val, word src_op) {
	if (prev_val > src_op) setFlag(Flag::C);	// carry bit set add
	else remFlag(Flag::C);
}

void cpu8086::testFlagCSubW(word prev_val, word src_op) {
	if (prev_val < src_op) setFlag(Flag::C);	// carry bit set sub
	else remFlag(Flag::C);
}

void cpu8086::testFlagAAdd(word prev_val, word src_op) {
	if ((prev_val & 0x000F) > (src_op & 0x000F)) setFlag(Flag::A);	// Auxiliary carry bit set add
	else remFlag(Flag::A);
}

void cpu8086::testFlagASub(word prev_val, word src_op) {
	if ((prev_val & 0x000F) < (src_op & 0x000F)) setFlag(Flag::A);	// Auxiliary carry bit set sub
	else remFlag(Flag::A);
}

void cpu8086::testFlagO(bool prev_sig_bit, bool now_sig_bit) {
	if (prev_sig_bit != now_sig_bit) setFlag(Flag::O);
	else remFlag(Flag::O);
}

// операции с флагами
bool cpu8086::getFlag(Flag f) { return (flag_reg >> (word)f) & 1; }
void cpu8086::setFlag(Flag f) { flag_reg |= (1 << (word)f); }
void cpu8086::remFlag(Flag f) { flag_reg &= (~(1 << (word)f)); }

// получение эффективного адреса в зависимости от типа адресации
word cpu8086::fetchEA(byte MOD, byte RM, word disp) {
	word effective_address = 0;
	switch (MOD) {
	case 0:	// MOD 00; no displacement
		switch (RM) {
		case 0: effective_address = B.X + SI; break;
		case 1: effective_address = B.X + DI; break;
		case 2: effective_address = BP + SI; break;
		case 3: effective_address = BP + DI; break;
		case 4: effective_address = SI; break;
		case 5: effective_address = DI; break;
		case 6: effective_address = disp; break;	// direct addressing
		case 7: effective_address = B.X; break;
		}
		break;
	case 1:	// MOD 01; 8-bit displacement
		switch (RM) {
		case 0: effective_address = B.X + SI + disp; break;
		case 1: effective_address = B.X + DI + disp; break;
		case 2: effective_address = BP + SI + disp; break;
		case 3: effective_address = BP + DI + disp; break;
		case 4: effective_address = SI + disp; break;
		case 5: effective_address = DI + disp; break;
		case 6: effective_address = BP + disp; break;
		case 7: effective_address = B.X + disp; break;
		}
		break;
	case 2:	// MOD 10; 16-bit displacement
		switch (RM) {
		case 0: effective_address = B.X + SI + disp; break;
		case 1: effective_address = B.X + DI + disp; break;
		case 2: effective_address = BP + SI + disp; break;
		case 3: effective_address = BP + DI + disp; break;
		case 4: effective_address = SI + disp; break;
		case 5: effective_address = DI + disp; break;
		case 6: effective_address = BP + disp; break;
		case 7: effective_address = B.X + disp; break;
		}
		break;
	//case 3:	// MOD 11; register addressing not supported
	}
	return effective_address;
}

word cpu8086::fetchDisp(byte mod, byte rm) {
	word displacement = 0;
	switch (mod) {
	case 0:	// нет смещения, но возможно действует режим прямой адресации
		if (rm == 0b110) {
			IP++;
			address = ((dword)CS << 4) + IP;
			displacement = memory->readW(address);	// при прямой адресации читаются два байта
			IP++;	// так как было считано два байта, нужно перевести указатель дальше
		}
		break;
	case 1: // однобайтное смещение
		IP++;	// переводим указатель на позицию вперёд
		address = ((dword)CS << 4) + IP;
		displacement = memory->readB(address);	// читаем байт
		break;
	case 2:	// двухбайтное смещение
		IP++;
		address = ((dword)CS << 4) + IP;
		displacement = memory->readW(address);
		IP++;
		break;
	}
	return displacement;
}

// декодирование регистра по номеру
// команды должны сами выбирать тип декодирования (8-битные 
// части регистров или весь регистр)
// возвращают ссылки на регистр
byte& cpu8086::getRegB(byte reg) {
	switch (reg) {
	case 0: return A.L; break;
	case 1: return C.L; break;
	case 2: return D.L; break;
	case 3: return B.L; break;
	case 4: return A.H; break;
	case 5: return C.H; break;
	case 6: return D.H; break;
	case 7: return B.H; break;
	}
}

word& cpu8086::getRegW(byte reg) {
	switch (reg) {
	case 0: return A.X; break;
	case 1: return C.X; break;
	case 2: return D.X; break;
	case 3: return B.X; break;
	case 4: return SP; break;
	case 5: return BP; break;
	case 6: return SI; break;
	case 7: return DI; break;
	}
}

// функция инициализирует таблицу команд
void cpu8086::initOpTable() {
	// сложение
	opcode_table[0x00] = std::bind(&cpu8086::ADD_R_OUT_B, this);
	opcode_table[0x01] = std::bind(&cpu8086::ADD_R_OUT_W, this);
	opcode_table[0x02] = std::bind(&cpu8086::ADD_R_IN_B, this);
	opcode_table[0x03] = std::bind(&cpu8086::ADD_R_IN_W, this);
	opcode_table[0x04] = std::bind(&cpu8086::ADD_A_B, this);
	opcode_table[0x05] = std::bind(&cpu8086::ADD_A_W, this);
	opcode_table[0x06] = std::bind(&cpu8086::PUSH_R, this, std::ref(ES));
	opcode_table[0x07] = std::bind(&cpu8086::POP_R, this, std::ref(ES));

	// инкремент регистров
	opcode_table[0x40] = std::bind(&cpu8086::INC_R, this, std::ref(A.X));
	opcode_table[0x41] = std::bind(&cpu8086::INC_R, this, std::ref(B.X));
	opcode_table[0x42] = std::bind(&cpu8086::INC_R, this, std::ref(C.X));
	opcode_table[0x43] = std::bind(&cpu8086::INC_R, this, std::ref(D.X));
	opcode_table[0x44] = std::bind(&cpu8086::INC_R, this, std::ref(SP));
	opcode_table[0x45] = std::bind(&cpu8086::INC_R, this, std::ref(BP));
	opcode_table[0x46] = std::bind(&cpu8086::INC_R, this, std::ref(SI));
	opcode_table[0x47] = std::bind(&cpu8086::INC_R, this, std::ref(DI));
	// декремент регистров
	opcode_table[0x48] = std::bind(&cpu8086::DEC_R, this, std::ref(A.X));
	opcode_table[0x49] = std::bind(&cpu8086::DEC_R, this, std::ref(B.X));
	opcode_table[0x4A] = std::bind(&cpu8086::DEC_R, this, std::ref(C.X));
	opcode_table[0x4B] = std::bind(&cpu8086::DEC_R, this, std::ref(D.X));
	opcode_table[0x4C] = std::bind(&cpu8086::DEC_R, this, std::ref(SP));
	opcode_table[0x4D] = std::bind(&cpu8086::DEC_R, this, std::ref(BP));
	opcode_table[0x4E] = std::bind(&cpu8086::DEC_R, this, std::ref(SI));
	opcode_table[0x4F] = std::bind(&cpu8086::DEC_R, this, std::ref(DI));
	// запись в стек
	opcode_table[0x50] = std::bind(&cpu8086::PUSH_R, this, std::ref(A.X));
	opcode_table[0x51] = std::bind(&cpu8086::PUSH_R, this, std::ref(B.X));
	opcode_table[0x52] = std::bind(&cpu8086::PUSH_R, this, std::ref(C.X));
	opcode_table[0x53] = std::bind(&cpu8086::PUSH_R, this, std::ref(D.X));
	opcode_table[0x54] = std::bind(&cpu8086::PUSH_R, this, std::ref(SP));
	opcode_table[0x55] = std::bind(&cpu8086::PUSH_R, this, std::ref(BP));
	opcode_table[0x56] = std::bind(&cpu8086::PUSH_R, this, std::ref(SI));
	opcode_table[0x57] = std::bind(&cpu8086::PUSH_R, this, std::ref(DI));
	// чтение из стека
	opcode_table[0x58] = std::bind(&cpu8086::POP_R, this, std::ref(A.X));
	opcode_table[0x59] = std::bind(&cpu8086::POP_R, this, std::ref(B.X));
	opcode_table[0x5A] = std::bind(&cpu8086::POP_R, this, std::ref(C.X));
	opcode_table[0x5B] = std::bind(&cpu8086::POP_R, this, std::ref(D.X));
	opcode_table[0x5C] = std::bind(&cpu8086::POP_R, this, std::ref(SP));
	opcode_table[0x5D] = std::bind(&cpu8086::POP_R, this, std::ref(BP));
	opcode_table[0x5E] = std::bind(&cpu8086::POP_R, this, std::ref(SI));
	opcode_table[0x5F] = std::bind(&cpu8086::POP_R, this, std::ref(DI));
}

/******OPCODES_BEG******/
void cpu8086::ADD_R_IN_B() {
	IP++;
	address = ((dword)CS << 4) + IP;	// генерация физического адреса для второго байта
	byte mod_reg_rm = memory->readB(address);
	// выделение полей
	byte mod = mod_reg_rm >> 6;
	byte reg = (mod_reg_rm & 0b00111000) >> 3;
	byte rm = (mod_reg_rm & 0b00000111);
	// определяем регистр для байтов
	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = first_reg;
	bool prev_sig_bit = getFlag(Flag::S);

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		first_reg += second_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg += memory->readB(address);
	}
	// установка флагов
	testFlagZ(first_reg);
	testFlagSB(first_reg);
	testFlagPB(first_reg);
	testFlagCAddB(prev_val, first_reg);
	testFlagAAdd(prev_val, first_reg);
	testFlagO(prev_sig_bit, getFlag(Flag::S));
}

void cpu8086::ADD_R_OUT_B() {
	IP++;
	address = ((dword)CS << 4) + IP;	// генерация физического адреса для второго байта
	byte mod_reg_rm = memory->readB(address);
	// выделение полей
	byte mod = mod_reg_rm >> 6;
	byte reg = (mod_reg_rm & 0b00111000) >> 3;
	byte rm = (mod_reg_rm & 0b00000111);
	
	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = 0;
	byte new_val = 0;
	bool prev_sig_bit = getFlag(Flag::S);

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		second_reg += first_reg;	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readB(address);
		new_val = prev_val + first_reg;
		memory->writeB(address, new_val);
	}
	testFlagZ(new_val);
	testFlagSB(new_val);
	testFlagPB(new_val);
	testFlagCAddB(prev_val, new_val);
	testFlagAAdd(prev_val, new_val);
	testFlagO(prev_sig_bit, getFlag(Flag::S));
}

void cpu8086::ADD_R_IN_W() {
	IP++;
	address = ((dword)CS << 4) + IP;	// генерация физического адреса для второго байта
	byte mod_reg_rm = memory->readB(address);
	// выделение полей
	byte mod = mod_reg_rm >> 6;
	byte reg = (mod_reg_rm & 0b00111000) >> 3;
	byte rm = (mod_reg_rm & 0b00000111);
	
	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = first_reg;
	bool prev_sig_bit = getFlag(Flag::S);

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		first_reg += second_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg += memory->readW(address);
	}
	testFlagZ(first_reg);
	testFlagSW(first_reg);
	testFlagPW(first_reg);
	testFlagCAddW(prev_val, first_reg);
	testFlagAAdd(prev_val, first_reg);
	testFlagO(prev_sig_bit, getFlag(Flag::S));
}

void cpu8086::ADD_R_OUT_W() {
	IP++;
	address = ((dword)CS << 4) + IP;	// генерация физического адреса для второго байта
	byte mod_reg_rm = memory->readB(address);
	// выделение полей
	byte mod = mod_reg_rm >> 6;
	byte reg = (mod_reg_rm & 0b00111000) >> 3;
	byte rm = (mod_reg_rm & 0b00000111);
	
	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = 0;
	word new_val = 0;
	bool prev_sig_bit = getFlag(Flag::S);

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		second_reg += first_reg;	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readW(address);
		new_val = prev_val + first_reg;
		memory->writeW(address, new_val);
	}
	testFlagZ(new_val);
	testFlagSW(new_val);
	testFlagPW(new_val);
	testFlagCAddW(prev_val, new_val);
	testFlagAAdd(prev_val, new_val);
	testFlagO(prev_sig_bit, getFlag(Flag::S));
}

void cpu8086::ADD_A_B() {
	IP++;
	address = ((dword)CS << 4) + IP;
	byte data = memory->readB(address);	// получение данных

	byte prev_val = A.L;
	bool prev_sig_bit = getFlag(Flag::S);
	A.L += data;

	testFlagZ(A.L);
	testFlagSB(A.L);
	testFlagPB(A.L);
	testFlagCAddB(prev_val, A.L);
	testFlagAAdd(prev_val, A.L);
	testFlagO(prev_sig_bit, getFlag(Flag::S));
}

void cpu8086::ADD_A_W() {
	IP++;
	address = ((dword)CS << 4) + IP;
	word data = memory->readW(address);
	IP++;

	word prev_val = A.X;
	bool prev_sig_bit = getFlag(Flag::S);
	A.X += data;

	testFlagZ(A.X);
	testFlagSW(A.X);
	testFlagPW(A.X);
	testFlagCAddW(prev_val, A.X);
	testFlagAAdd(prev_val, A.X);
	testFlagO(prev_sig_bit, getFlag(Flag::S));
}

void cpu8086::INC_R(word& rgs) {
	word prev_val = rgs;
	bool prev_sig = getFlag(Flag::S);
	rgs++;
	// affected flags
	testFlagZ(rgs);
	testFlagSW(rgs);
	testFlagPW(rgs);
	testFlagAAdd(prev_val, rgs);
	bool now_sig = getFlag(Flag::S);
	testFlagO(prev_sig, now_sig);
}

void cpu8086::DEC_R(word& rgs) {
	word prev_val = rgs;
	bool prev_sig = getFlag(Flag::S);
	rgs--;
	// affected flags
	testFlagZ(rgs);
	testFlagSW(rgs);
	testFlagPW(rgs);
	testFlagASub(prev_val, rgs);
	bool now_sig = getFlag(Flag::S);
	testFlagO(prev_sig, now_sig);
}

void cpu8086::PUSH_R(word& rgs) {
	SP -= 2;	// стек растёт вниз
	address = ((dword)SS << 4) + SP;	// получение физического адреса вершины стека
	memory->writeStack(address, rgs);	
}

void cpu8086::POP_R(word& rgs) {
	address = ((dword)SS << 4) + SP;
	rgs = memory->readStack(address);
	SP += 2;
}
/******OPCODES_END******/