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

	presenter = new EmptyPresenter();
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
	presenter->notifyRegChange();		// оповещение презентатора об изменениях
}

void cpu8086::initMemory(std::shared_ptr<Memory> mem) {
	memory = mem;
}

void cpu8086::initPresenter(AbstractPresenter* p_pres) {
	presenter = p_pres;
}

void cpu8086::loadDefaultState() {
	// инициализация базовых регистров
	setRegVal(RegId::AX, 0x00);
	setRegVal(RegId::BX, 0x00);
	setRegVal(RegId::CX, 0x00);
	setRegVal(RegId::DX, 0x00);
	setRegVal(RegId::SP, 0x00);
	setRegVal(RegId::BP, 0x00);
	setRegVal(RegId::SI, 0x00);
	setRegVal(RegId::DI, 0x00);
	// инициализация сегментов
	setRegVal(RegId::CS, 0x1000);
	setRegVal(RegId::DS, 0x2000);
	setRegVal(RegId::SS, 0x3000);
	setRegVal(RegId::IP, 0x00);
	// инициализация флагов
	remFlag(Flag::O);
	remFlag(Flag::S);
	remFlag(Flag::Z);
	remFlag(Flag::A);
	remFlag(Flag::P);
	remFlag(Flag::C);
}

word cpu8086::getRegVal(RegId reg_id) {
	switch (reg_id) {
	case RegId::AL: return A.low_high[0]; case RegId::AH: return A.low_high[1]; case RegId::AX: return A.X;
	case RegId::BL: return B.low_high[0]; case RegId::BH: return B.low_high[1]; case RegId::BX: return B.X;
	case RegId::CL: return C.low_high[0]; case RegId::CH: return C.low_high[1]; case RegId::CX: return C.X;
	case RegId::DL: return D.low_high[0]; case RegId::DH: return D.low_high[1]; case RegId::DX: return D.X;
	case RegId::SP: return SP; case RegId::BP: return BP; 
	case RegId::SI: return SI; case RegId::DI: return DI;
	case RegId::CS: return CS; case RegId::DS: return DS;
	case RegId::SS: return SS; case RegId::ES: return ES;
	case RegId::IP: return IP;
	}
}

void cpu8086::setRegVal(RegId reg_id, word value) {
	switch (reg_id) {
	case RegId::AL: A.low_high[0] = value; break; 
	case RegId::AH: A.low_high[1] = value; break;
	case RegId::AX: A.X = value; break;
	case RegId::BL: B.low_high[0] = value; break; 
	case RegId::BH: B.low_high[1] = value; break;
	case RegId::BX: B.X = value; break;
	case RegId::CL: C.low_high[0] = value; break; 
	case RegId::CH: C.low_high[1] = value; break;
	case RegId::CX: C.X = value; break;
	case RegId::DL: D.low_high[0] = value; break; 
	case RegId::DH: D.low_high[1] = value; break;
	case RegId::DX: D.X = value; break;
	case RegId::SP: SP = value; break; 
	case RegId::BP: BP = value; break;
	case RegId::SI: SI = value; break; 
	case RegId::DI: DI = value; break;
	case RegId::CS: CS = value; break; 
	case RegId::DS: DS = value; break;
	case RegId::SS: SS = value; break; 
	case RegId::ES: ES = value; break;
	case RegId::IP: IP = value; break;
	}
}


// фкнкции проверки флагов
void cpu8086::testFlagZ(word new_val) {
	(new_val == 0) ? setFlag(Flag::Z) : remFlag(Flag::Z);
}

void cpu8086::testFlagS(word new_val, OpType type) {
	switch (type) {
		case OpType::Byte:
			(new_val >> (sizeof(byte) * 8 - 1) & 1) ? setFlag(Flag::S) : remFlag(Flag::S);
			break;
		case OpType::Word:
			(new_val >> (sizeof(word) * 8 - 1) & 1) ? setFlag(Flag::S) : remFlag(Flag::S);
			break;
	}
}

void cpu8086::testFlagP(byte val) {
	word set_bits = 0;
	for (int i = 0; i < sizeof(byte) * 8; i++) {
		if ((val & 1) == 1) set_bits++;
		val = val >> 1;
	}
	if (set_bits % 2 == 0) setFlag(Flag::P);	// parity bit set
	else remFlag(Flag::P);
}

void cpu8086::testFlagCAdd(word prev_val, word new_val) {
	if (prev_val > new_val) setFlag(Flag::C);	// carry bit set add
	else remFlag(Flag::C);
}

void cpu8086::testFlagCSub(word prev_val, word new_val) {
	if (prev_val < new_val) setFlag(Flag::C);	// carry bit set sub
	else remFlag(Flag::C);
}

void cpu8086::testFlagAAdd(word prev_val, word new_val) {
	if ((prev_val & 0x000F) > (new_val & 0x000F)) setFlag(Flag::A);	// Auxiliary carry bit set add
	else remFlag(Flag::A);
}

void cpu8086::testFlagASub(word prev_val, word new_val) {
	if ((prev_val & 0x000F) < (new_val & 0x000F)) setFlag(Flag::A);	// Auxiliary carry bit set sub
	else remFlag(Flag::A);
}

void cpu8086::testFlagO(word prev_val, word new_val, OpType type) {
	switch (type) {
	case OpType::Byte:
		if (((prev_val >> sizeof(byte) * 8 - 1)) != 
			((new_val >> sizeof(byte) * 8 - 1)) && !getFlag(Flag::C))
			setFlag(Flag::O);
		else remFlag(Flag::O);
		break;
	case OpType::Word:
		if (((prev_val >> sizeof(word) * 8 - 1)) != 
			((new_val >> sizeof(word) * 8 - 1)) && !getFlag(Flag::C))
			setFlag(Flag::O);
		else remFlag(Flag::O);
		break;
	}
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
			displacement = fetchCodeWord();
		}
		break;
	case 1: // однобайтное смещение
		displacement = fetchCodeByte();
		break;
	case 2:	// двухбайтное смещение
		displacement = fetchCodeWord();
		break;
	}
	return displacement;
}

byte cpu8086::fetchCodeByte() {
	IP++;
	return memory->readB(((dword)CS << 4) + IP);
}

word cpu8086::fetchCodeWord() {
	IP++;
	word ret_val = memory->readW(((dword)CS << 4) + IP);
	IP++;
	return ret_val;
}

void cpu8086::fetchModRegRm(byte& mod, byte& reg, byte& rm) {
	byte mod_reg_rm = fetchCodeByte();	// получение следующего байта после opcode'а
	// выделение полей
	mod = mod_reg_rm >> 6;
	reg = (mod_reg_rm & 0b00111000) >> 3;
	rm = (mod_reg_rm & 0b00000111);
}

// декодирование регистра по номеру
// команды должны сами выбирать тип декодирования (8-битные 
// части регистров или весь регистр)
// возвращают ссылки на регистр
byte& cpu8086::getRegB(byte reg) {
	switch (reg) {
	case 0: return A.low_high[0]; break;
	case 1: return C.low_high[0]; break;
	case 2: return D.low_high[0]; break;
	case 3: return B.low_high[0]; break;
	case 4: return A.low_high[1]; break;
	case 5: return C.low_high[1]; break;
	case 6: return D.low_high[1]; break;
	case 7: return B.low_high[1]; break;
	default: return A.low_high[0]; break;
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
	default: return A.X; break;
	}
}

word& cpu8086::getSegReg(byte reg) {
	switch(reg) {
	case 0: return ES;  break;
	case 1: return CS;  break;
	case 2: return SS;  break;
	case 3: return DS;  break;
	default: return ES; break;
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
	// логическое сложение
	opcode_table[0x08] = std::bind(&cpu8086::OR_R_OUT_B, this);
	opcode_table[0x09] = std::bind(&cpu8086::OR_R_OUT_W, this);
	opcode_table[0x0A] = std::bind(&cpu8086::OR_R_IN_B, this);
	opcode_table[0x0B] = std::bind(&cpu8086::OR_R_IN_W, this);
	opcode_table[0x0C] = std::bind(&cpu8086::OR_A_B, this);
	opcode_table[0x0D] = std::bind(&cpu8086::OR_A_W, this);
	opcode_table[0x0E] = std::bind(&cpu8086::PUSH_R, this, std::ref(CS));
	// сложение с carry битом
	opcode_table[0x10] = std::bind(&cpu8086::ADC_R_OUT_B, this);
	opcode_table[0x11] = std::bind(&cpu8086::ADC_R_OUT_W, this);
	opcode_table[0x12] = std::bind(&cpu8086::ADC_R_IN_B, this);
	opcode_table[0x13] = std::bind(&cpu8086::ADC_R_IN_W, this);
	opcode_table[0x14] = std::bind(&cpu8086::ADC_A_B, this);
	opcode_table[0x15] = std::bind(&cpu8086::ADC_A_W, this);
	opcode_table[0x16] = std::bind(&cpu8086::PUSH_R, this, std::ref(SS));
	opcode_table[0x17] = std::bind(&cpu8086::POP_R, this, std::ref(SS));
	// вычитание с carry битом
	opcode_table[0x18] = std::bind(&cpu8086::SBB_R_OUT_B, this);
	opcode_table[0x19] = std::bind(&cpu8086::SBB_R_OUT_W, this);
	opcode_table[0x1A] = std::bind(&cpu8086::SBB_R_IN_B, this);
	opcode_table[0x1B] = std::bind(&cpu8086::SBB_R_IN_W, this);
	opcode_table[0x1C] = std::bind(&cpu8086::SBB_A_B, this);
	opcode_table[0x1D] = std::bind(&cpu8086::SBB_A_W, this);
	opcode_table[0x1E] = std::bind(&cpu8086::PUSH_R, this, std::ref(DS));
	opcode_table[0x1F] = std::bind(&cpu8086::POP_R, this, std::ref(DS));
	// логическое умножение
	opcode_table[0x20] = std::bind(&cpu8086::AND_R_OUT_B, this);
	opcode_table[0x21] = std::bind(&cpu8086::AND_R_OUT_W, this);
	opcode_table[0x22] = std::bind(&cpu8086::AND_R_IN_B, this);
	opcode_table[0x23] = std::bind(&cpu8086::AND_R_IN_W, this);
	opcode_table[0x24] = std::bind(&cpu8086::AND_A_B, this);
	opcode_table[0x25] = std::bind(&cpu8086::AND_A_W, this);
	// DAA
	opcode_table[0x27] = std::bind(&cpu8086::DAA, this);
	// вычитание
	opcode_table[0x28] = std::bind(&cpu8086::SUB_R_OUT_B, this);
	opcode_table[0x29] = std::bind(&cpu8086::SUB_R_OUT_W, this);
	opcode_table[0x2A] = std::bind(&cpu8086::SUB_R_IN_B, this);
	opcode_table[0x2B] = std::bind(&cpu8086::SUB_R_IN_W, this);
	opcode_table[0x2C] = std::bind(&cpu8086::SUB_A_B, this);
	opcode_table[0x2D] = std::bind(&cpu8086::SUB_A_W, this);
	// DAS
	opcode_table[0x2F] = std::bind(&cpu8086::DAS, this);
	// логическое исключение
	opcode_table[0x30] = std::bind(&cpu8086::XOR_R_OUT_B, this);
	opcode_table[0x31] = std::bind(&cpu8086::XOR_R_OUT_W, this);
	opcode_table[0x32] = std::bind(&cpu8086::XOR_R_IN_B, this);
	opcode_table[0x33] = std::bind(&cpu8086::XOR_R_IN_W, this);
	opcode_table[0x34] = std::bind(&cpu8086::XOR_A_B, this);
	opcode_table[0x35] = std::bind(&cpu8086::XOR_A_W, this);
	// AAA
	opcode_table[0x37] = std::bind(&cpu8086::AAA, this);
	// сравнение
	opcode_table[0x38] = std::bind(&cpu8086::CMP_R_OUT_B, this);
	opcode_table[0x39] = std::bind(&cpu8086::CMP_R_OUT_W, this);
	opcode_table[0x3A] = std::bind(&cpu8086::CMP_R_IN_B, this);
	opcode_table[0x3B] = std::bind(&cpu8086::CMP_R_IN_W, this);
	opcode_table[0x3C] = std::bind(&cpu8086::CMP_A_B, this);
	opcode_table[0x3D] = std::bind(&cpu8086::CMP_A_W, this);
	// AAS
	opcode_table[0x3F] = std::bind(&cpu8086::AAS, this);
	// инкремент регистров
	opcode_table[0x40] = std::bind(&cpu8086::INC_R, this, std::ref(A.X));
	opcode_table[0x41] = std::bind(&cpu8086::INC_R, this, std::ref(C.X));
	opcode_table[0x42] = std::bind(&cpu8086::INC_R, this, std::ref(D.X));
	opcode_table[0x43] = std::bind(&cpu8086::INC_R, this, std::ref(B.X));
	opcode_table[0x44] = std::bind(&cpu8086::INC_R, this, std::ref(SP));
	opcode_table[0x45] = std::bind(&cpu8086::INC_R, this, std::ref(BP));
	opcode_table[0x46] = std::bind(&cpu8086::INC_R, this, std::ref(SI));
	opcode_table[0x47] = std::bind(&cpu8086::INC_R, this, std::ref(DI));
	// декремент регистров
	opcode_table[0x48] = std::bind(&cpu8086::DEC_R, this, std::ref(A.X));
	opcode_table[0x49] = std::bind(&cpu8086::DEC_R, this, std::ref(C.X));
	opcode_table[0x4A] = std::bind(&cpu8086::DEC_R, this, std::ref(D.X));
	opcode_table[0x4B] = std::bind(&cpu8086::DEC_R, this, std::ref(B.X));
	opcode_table[0x4C] = std::bind(&cpu8086::DEC_R, this, std::ref(SP));
	opcode_table[0x4D] = std::bind(&cpu8086::DEC_R, this, std::ref(BP));
	opcode_table[0x4E] = std::bind(&cpu8086::DEC_R, this, std::ref(SI));
	opcode_table[0x4F] = std::bind(&cpu8086::DEC_R, this, std::ref(DI));
	// запись в стек
	opcode_table[0x50] = std::bind(&cpu8086::PUSH_R, this, std::ref(A.X));
	opcode_table[0x51] = std::bind(&cpu8086::PUSH_R, this, std::ref(C.X));
	opcode_table[0x52] = std::bind(&cpu8086::PUSH_R, this, std::ref(D.X));
	opcode_table[0x53] = std::bind(&cpu8086::PUSH_R, this, std::ref(B.X));
	opcode_table[0x54] = std::bind(&cpu8086::PUSH_R, this, std::ref(SP));
	opcode_table[0x55] = std::bind(&cpu8086::PUSH_R, this, std::ref(BP));
	opcode_table[0x56] = std::bind(&cpu8086::PUSH_R, this, std::ref(SI));
	opcode_table[0x57] = std::bind(&cpu8086::PUSH_R, this, std::ref(DI));
	// чтение из стека
	opcode_table[0x58] = std::bind(&cpu8086::POP_R, this, std::ref(A.X));
	opcode_table[0x59] = std::bind(&cpu8086::POP_R, this, std::ref(C.X));
	opcode_table[0x5A] = std::bind(&cpu8086::POP_R, this, std::ref(D.X));
	opcode_table[0x5B] = std::bind(&cpu8086::POP_R, this, std::ref(B.X));
	opcode_table[0x5C] = std::bind(&cpu8086::POP_R, this, std::ref(SP));
	opcode_table[0x5D] = std::bind(&cpu8086::POP_R, this, std::ref(BP));
	opcode_table[0x5E] = std::bind(&cpu8086::POP_R, this, std::ref(SI));
	opcode_table[0x5F] = std::bind(&cpu8086::POP_R, this, std::ref(DI));
	// условный переход
	opcode_table[0x70] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return getFlag(Flag::O); });	// JO
	opcode_table[0x71] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return !getFlag(Flag::O); }); // JNO
	opcode_table[0x72] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return getFlag(Flag::C); });	// JB/JNAE
	opcode_table[0x73] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return !getFlag(Flag::C); }); // JNB/JAE
	opcode_table[0x74] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return getFlag(Flag::Z); });	// JE/JZ
	opcode_table[0x75] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return !getFlag(Flag::Z); }); // JNE/JNZ
	opcode_table[0x76] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return (getFlag(Flag::C) || getFlag(Flag::Z)); });	// JBE/JNA
	opcode_table[0x77] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return !(getFlag(Flag::C) || getFlag(Flag::Z)); });	// JA/JNBE
	opcode_table[0x78] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return getFlag(Flag::S); });	// JS
	opcode_table[0x79] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return !getFlag(Flag::S); });	// JNS
	opcode_table[0x7A] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return getFlag(Flag::P); });	// JP/JPE
	opcode_table[0x7B] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return !getFlag(Flag::P); });	// JNP/JPO
	opcode_table[0x7C] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return (getFlag(Flag::S) ^ getFlag(Flag::O)); });		// JL/JNGE
	opcode_table[0x7D] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return !(getFlag(Flag::S) ^ getFlag(Flag::O)); });	// JNL/JGE
	opcode_table[0x7E] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return ((getFlag(Flag::S) ^ getFlag(Flag::O)) || getFlag(Flag::Z)); });	// JLE/JNG
	opcode_table[0x7F] = std::bind(&cpu8086::JMP_COND, this, 
		[&] { return !((getFlag(Flag::S) ^ getFlag(Flag::O)) || getFlag(Flag::Z)); });	// JNLE/JG
	// прямое значение
	opcode_table[0x80] = std::bind(&cpu8086::IMMED_B, this);
	opcode_table[0x81] = std::bind(&cpu8086::IMMED_W, this);
	opcode_table[0x82] = std::bind(&cpu8086::IMMED_B_SX, this);
	opcode_table[0x83] = std::bind(&cpu8086::IMMED_W_SX, this);
	// обмен
	opcode_table[0x86] = std::bind(&cpu8086::XCHG_B, this);
	opcode_table[0x87] = std::bind(&cpu8086::XCHG_W, this);
	// помещение значения из регистра в память
	opcode_table[0x88] = std::bind(&cpu8086::MOV_R_OUT_B, this);
	opcode_table[0x89] = std::bind(&cpu8086::MOV_R_OUT_W, this);
	// помещение из и в сегментные регистры
	opcode_table[0x8C] = std::bind(&cpu8086::MOV_SR_OUT, this);
	opcode_table[0x8E] = std::bind(&cpu8086::MOV_SR_IN, this);
	// помещение значения в регистр из памяти
	opcode_table[0x8A] = std::bind(&cpu8086::MOV_R_IN_B, this);
	opcode_table[0x8B] = std::bind(&cpu8086::MOV_R_IN_W, this);
	// помещение значения из аккумулятора в память
	opcode_table[0xA0] = std::bind(&cpu8086::MOV_A_IN_B, this);
	opcode_table[0xA1] = std::bind(&cpu8086::MOV_A_IN_W, this);
	// помещение значения в аккумулятор из памяти
	opcode_table[0xA2] = std::bind(&cpu8086::MOV_A_OUT_B, this);
	opcode_table[0xA3] = std::bind(&cpu8086::MOV_A_OUT_W, this);
	// прямое помещение значения в байтовый регистр
	opcode_table[0xB0] = std::bind(&cpu8086::MOV_R_IMM_B, this, std::ref(A.low_high[0]));
	opcode_table[0xB1] = std::bind(&cpu8086::MOV_R_IMM_B, this, std::ref(C.low_high[0]));
	opcode_table[0xB2] = std::bind(&cpu8086::MOV_R_IMM_B, this, std::ref(D.low_high[0]));
	opcode_table[0xB3] = std::bind(&cpu8086::MOV_R_IMM_B, this, std::ref(B.low_high[0]));
	opcode_table[0xB4] = std::bind(&cpu8086::MOV_R_IMM_B, this, std::ref(A.low_high[1]));
	opcode_table[0xB5] = std::bind(&cpu8086::MOV_R_IMM_B, this, std::ref(C.low_high[1]));
	opcode_table[0xB6] = std::bind(&cpu8086::MOV_R_IMM_B, this, std::ref(D.low_high[1]));
	opcode_table[0xB7] = std::bind(&cpu8086::MOV_R_IMM_B, this, std::ref(B.low_high[1]));
	// прямое помещение значения в двухбайтовый регистр
	opcode_table[0xB8] = std::bind(&cpu8086::MOV_R_IMM_W, this, std::ref(A.X));
	opcode_table[0xB9] = std::bind(&cpu8086::MOV_R_IMM_W, this, std::ref(C.X));
	opcode_table[0xBA] = std::bind(&cpu8086::MOV_R_IMM_W, this, std::ref(D.X));
	opcode_table[0xBB] = std::bind(&cpu8086::MOV_R_IMM_W, this, std::ref(B.X));
	opcode_table[0xBC] = std::bind(&cpu8086::MOV_R_IMM_W, this, std::ref(SP));
	opcode_table[0xBD] = std::bind(&cpu8086::MOV_R_IMM_W, this, std::ref(BP));
	opcode_table[0xBE] = std::bind(&cpu8086::MOV_R_IMM_W, this, std::ref(SI));
	opcode_table[0xBF] = std::bind(&cpu8086::MOV_R_IMM_W, this, std::ref(DI));
	// прямое помещение значения в память
	opcode_table[0xC6] = std::bind(&cpu8086::MOV_MEM_IMM_B, this);
	opcode_table[0xC7] = std::bind(&cpu8086::MOV_MEM_IMM_W, this);
	// halt
	opcode_table[0xF4] = std::bind(&cpu8086::HLT, this);
}

/******OPCODES_BEG******/
void cpu8086::ADD_R_OUT_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = 0;
	byte new_val = 0;
	

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
	testFlagS(new_val, OpType::Byte);
	testFlagP(new_val);
	testFlagCAdd(prev_val, new_val);
	testFlagAAdd(prev_val, new_val);
	testFlagO(prev_val, new_val, OpType::Byte);
}

void cpu8086::ADD_R_OUT_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = 0;
	word new_val = 0;
	

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
	testFlagS(new_val, OpType::Word);
	testFlagP(new_val);
	testFlagCAdd(prev_val, new_val);
	testFlagAAdd(prev_val, new_val);
	testFlagO(prev_val, new_val, OpType::Word);
}

void cpu8086::ADD_R_IN_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);
	// определяем регистр для байтов
	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = first_reg;
	

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
	testFlagS(first_reg, OpType::Byte);
	testFlagP(first_reg);
	testFlagCAdd(prev_val, first_reg);
	testFlagAAdd(prev_val, first_reg);
	testFlagO(prev_val, first_reg, OpType::Byte);
}

void cpu8086::ADD_R_IN_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);
	
	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = first_reg;
	

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
	testFlagS(first_reg, OpType::Word);
	testFlagP(first_reg);
	testFlagCAdd(prev_val, first_reg);
	testFlagAAdd(prev_val, first_reg);
	testFlagO(prev_val, first_reg, OpType::Word);
}

void cpu8086::ADD_A_B() {
	byte data = fetchCodeByte();

	byte prev_val = A.low_high[0];
	
	A.low_high[0] += data;

	testFlagZ(A.low_high[0]);
	testFlagS(A.low_high[0], OpType::Byte);
	testFlagP(A.low_high[0]);
	testFlagCAdd(prev_val, A.low_high[0]);
	testFlagAAdd(prev_val, A.low_high[0]);
	testFlagO(prev_val, A.low_high[0], OpType::Byte);
}

void cpu8086::ADD_A_W() {
	word data = fetchCodeWord();

	word prev_val = A.X;
	
	A.X += data;

	testFlagZ(A.X);
	testFlagS(A.X, OpType::Word);
	testFlagP(A.X);
	testFlagCAdd(prev_val, A.X);
	testFlagAAdd(prev_val, A.X);
	testFlagO(prev_val, A.X, OpType::Word);
}

void cpu8086::OR_R_OUT_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = 0;
	byte new_val = 0;
	

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		second_reg |= first_reg;	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readB(address);
		new_val = prev_val | first_reg;
		memory->writeB(address, new_val);
	}
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Byte);
	testFlagP(new_val);
	remFlag(Flag::C);
	(new_val > prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::OR_R_OUT_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = 0;
	word new_val = 0;
	

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		second_reg |= first_reg;	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readW(address);
		new_val = prev_val | first_reg;
		memory->writeW(address, new_val);
	}
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Word);
	testFlagP(new_val);
	remFlag(Flag::C);
	(new_val > prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::OR_R_IN_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);
	// определяем регистр для байтов
	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = first_reg;
	

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		first_reg |= second_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg |= memory->readB(address);
	}
	// установка флагов
	testFlagZ(first_reg);
	testFlagS(first_reg, OpType::Byte);
	testFlagP(first_reg);
	remFlag(Flag::C);
	(first_reg > prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::OR_R_IN_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = first_reg;
	

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		first_reg |= second_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg |= memory->readW(address);
	}
	testFlagZ(first_reg);
	testFlagS(first_reg, OpType::Word);
	testFlagP(first_reg);
	remFlag(Flag::C);
	(first_reg > prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::OR_A_B() {
	byte data = fetchCodeByte();

	byte prev_val = A.low_high[0];
	
	A.low_high[0] |= data;

	testFlagZ(A.low_high[0]);
	testFlagS(A.low_high[0], OpType::Byte);
	testFlagP(A.low_high[0]);
	remFlag(Flag::C);
	(A.low_high[0] > prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::OR_A_W() {
	word data = fetchCodeWord();

	word prev_val = A.X;
	
	A.X |= data;

	testFlagZ(A.X);
	testFlagS(A.X, OpType::Word);
	testFlagP(A.X);
	remFlag(Flag::C);
	(A.X > prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::ADC_R_OUT_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = 0;
	byte new_val = 0;
	

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		second_reg += first_reg + getFlag(Flag::C);	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readB(address);
		new_val = prev_val + first_reg + getFlag(Flag::C);
		memory->writeB(address, new_val);
	}
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Byte);
	testFlagP(new_val);
	testFlagCAdd(prev_val, new_val);
	testFlagAAdd(prev_val, new_val);
	testFlagO(prev_val, new_val, OpType::Byte);
}

void cpu8086::ADC_R_OUT_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = 0;
	word new_val = 0;
	

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		second_reg += first_reg + getFlag(Flag::C);	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readW(address);
		new_val = prev_val + first_reg + getFlag(Flag::C);
		memory->writeW(address, new_val);
	}
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Word);
	testFlagP(new_val);
	testFlagCAdd(prev_val, new_val);
	testFlagAAdd(prev_val, new_val);
	testFlagO(prev_val, new_val, OpType::Word);
}

void cpu8086::ADC_R_IN_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);
	// определяем регистр для байтов
	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = first_reg;
	

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		first_reg += second_reg + getFlag(Flag::C);
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg += memory->readB(address) + getFlag(Flag::C);
	}
	// установка флагов
	testFlagZ(first_reg);
	testFlagS(first_reg, OpType::Byte);
	testFlagP(first_reg);
	testFlagCAdd(prev_val, first_reg);
	testFlagAAdd(prev_val, first_reg);
	testFlagO(prev_val, first_reg, OpType::Byte);
}

void cpu8086::ADC_R_IN_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = first_reg;
	

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		first_reg += second_reg + getFlag(Flag::C);
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg += memory->readW(address) + getFlag(Flag::C);
	}
	testFlagZ(first_reg);
	testFlagS(first_reg, OpType::Word);
	testFlagP(first_reg);
	testFlagCAdd(prev_val, first_reg);
	testFlagAAdd(prev_val, first_reg);
	testFlagO(prev_val, first_reg, OpType::Word);
}

void cpu8086::ADC_A_B() {
	byte data = fetchCodeByte();

	byte prev_val = A.low_high[0];
	
	A.low_high[0] += data + getFlag(Flag::C);

	testFlagZ(A.low_high[0]);
	testFlagS(A.low_high[0], OpType::Byte);
	testFlagP(A.low_high[0]);
	testFlagCAdd(prev_val, A.low_high[0]);
	testFlagAAdd(prev_val, A.low_high[0]);
	testFlagO(prev_val, A.low_high[0], OpType::Byte);
}

void cpu8086::ADC_A_W() {
	word data = fetchCodeWord();

	word prev_val = A.X;
	
	A.X += data + getFlag(Flag::C);

	testFlagZ(A.X);
	testFlagS(A.X, OpType::Word);
	testFlagP(A.X);
	testFlagCAdd(prev_val, A.X);
	testFlagAAdd(prev_val, A.X);
	testFlagO(prev_val, A.X, OpType::Word);
}

void cpu8086::SBB_R_OUT_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = 0;
	byte new_val = 0;
	

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		second_reg = second_reg - first_reg - getFlag(Flag::C);	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readB(address);
		new_val = prev_val - first_reg - getFlag(Flag::C);
		memory->writeB(address, new_val);
	}
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Byte);
	testFlagP(new_val);
	testFlagCSub(prev_val, new_val);
	testFlagASub(prev_val, new_val);
	testFlagO(prev_val, new_val, OpType::Byte);
}

void cpu8086::SBB_R_OUT_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = 0;
	word new_val = 0;
	

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		second_reg = second_reg - first_reg - getFlag(Flag::C);	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readW(address);
		new_val = prev_val - first_reg - getFlag(Flag::C);
		memory->writeW(address, new_val);
	}
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Word);
	testFlagP(new_val);
	testFlagCSub(prev_val, new_val);
	testFlagASub(prev_val, new_val);
	testFlagO(prev_val, new_val, OpType::Word);
}

void cpu8086::SBB_R_IN_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);
	// определяем регистр для байтов
	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = first_reg;
	

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		first_reg = first_reg - second_reg - getFlag(Flag::C);
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg = first_reg - memory->readB(address) - getFlag(Flag::C);
	}
	// установка флагов
	testFlagZ(first_reg);
	testFlagS(first_reg, OpType::Byte);
	testFlagP(first_reg);
	testFlagCSub(prev_val, first_reg);
	testFlagASub(prev_val, first_reg);
	testFlagO(prev_val, first_reg, OpType::Byte);
}

void cpu8086::SBB_R_IN_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = first_reg;
	

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		first_reg = first_reg - second_reg - getFlag(Flag::C);
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg = first_reg - memory->readW(address) - getFlag(Flag::C);
	}
	testFlagZ(first_reg);
	testFlagS(first_reg, OpType::Word);
	testFlagP(first_reg);
	testFlagCSub(prev_val, first_reg);
	testFlagASub(prev_val, first_reg);
	testFlagO(prev_val, first_reg, OpType::Word);
}

void cpu8086::SBB_A_B() {
	byte data = fetchCodeByte();

	byte prev_val = A.low_high[0];
	
	A.low_high[0] = A.low_high[0] - data - getFlag(Flag::C);

	testFlagZ(A.low_high[0]);
	testFlagS(A.low_high[0], OpType::Byte);
	testFlagP(A.low_high[0]);
	testFlagCSub(prev_val, A.low_high[0]);
	testFlagASub(prev_val, A.low_high[0]);
	testFlagO(prev_val, A.low_high[0], OpType::Byte);
}

void cpu8086::SBB_A_W() {
	word data = fetchCodeWord();

	word prev_val = A.X;
	
	A.X = A.X - data - getFlag(Flag::C);

	testFlagZ(A.X);
	testFlagS(A.X, OpType::Word);
	testFlagP(A.X);
	testFlagCSub(prev_val, A.X);
	testFlagASub(prev_val, A.X);
	testFlagO(prev_val, A.X, OpType::Word);
}

void cpu8086::AND_R_OUT_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = 0;
	byte new_val = 0;
	

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		second_reg &= first_reg;	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readB(address);
		new_val = prev_val & first_reg;
		memory->writeB(address, new_val);
	}
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Byte);
	testFlagP(new_val);
	remFlag(Flag::C);
	(new_val == prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::AND_R_OUT_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = 0;
	word new_val = 0;
	

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		second_reg &= first_reg;	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readW(address);
		new_val = prev_val & first_reg;
		memory->writeW(address, new_val);
	}
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Word);
	testFlagP(new_val);
	remFlag(Flag::C);
	(new_val == prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::AND_R_IN_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);
	// определяем регистр для байтов
	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = first_reg;
	

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		first_reg &= second_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg &= memory->readB(address);
	}
	// установка флагов
	testFlagZ(first_reg);
	testFlagS(first_reg, OpType::Byte);
	testFlagP(first_reg);
	remFlag(Flag::C);
	(first_reg & prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::AND_R_IN_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = first_reg;
	

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		first_reg &= second_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg &= memory->readW(address);
	}
	testFlagZ(first_reg);
	testFlagS(first_reg, OpType::Word);
	testFlagP(first_reg);
	remFlag(Flag::C);
	(first_reg == prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::AND_A_B() {
	byte data = fetchCodeByte();

	byte prev_val = A.low_high[0];
	
	A.low_high[0] &= data;

	testFlagZ(A.low_high[0]);
	testFlagS(A.low_high[0], OpType::Byte);
	testFlagP(A.low_high[0]);
	remFlag(Flag::C);
	(A.low_high[0] == prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::AND_A_W() {
	word data = fetchCodeWord();

	word prev_val = A.X;
	
	A.X &= data;

	testFlagZ(A.X);
	testFlagS(A.X, OpType::Word);
	testFlagP(A.X);
	remFlag(Flag::C);
	(A.X == prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::DAA() {
	byte prev_val = A.low_high[0];
	if ((A.low_high[0] & 0xF) > 9 || getFlag(Flag::A)) {
		A.low_high[0] += 6;
		setFlag(Flag::A);
	}
	if (A.low_high[0] > 0x9F || getFlag(Flag::C)) {
		A.low_high[0] += 0x60;
		setFlag(Flag::C);
	}
	testFlagZ(A.low_high[0]);
	testFlagS(A.low_high[0], OpType::Byte);
	testFlagP(A.low_high[0]);
	testFlagO(prev_val, A.low_high[0], OpType::Byte);
}

void cpu8086::SUB_R_OUT_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = 0;
	byte new_val = 0;
	

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		second_reg = second_reg - first_reg;	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readB(address);
		new_val = prev_val - first_reg;
		memory->writeB(address, new_val);
	}
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Byte);
	testFlagP(new_val);
	testFlagCSub(prev_val, new_val);
	testFlagASub(prev_val, new_val);
	testFlagO(prev_val, new_val, OpType::Byte);
}

void cpu8086::SUB_R_OUT_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = 0;
	word new_val = 0;
	

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		second_reg = second_reg - first_reg;	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readW(address);
		new_val = prev_val - first_reg;
		memory->writeW(address, new_val);
	}
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Word);
	testFlagP(new_val);
	testFlagCSub(prev_val, new_val);
	testFlagASub(prev_val, new_val);
	testFlagO(prev_val, new_val, OpType::Word);
}

void cpu8086::SUB_R_IN_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);
	// определяем регистр для байтов
	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = first_reg;
	

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		first_reg = first_reg - second_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg = first_reg - memory->readB(address);
	}
	// установка флагов
	testFlagZ(first_reg);
	testFlagS(first_reg, OpType::Byte);
	testFlagP(first_reg);
	testFlagCSub(prev_val, first_reg);
	testFlagASub(prev_val, first_reg);
	testFlagO(prev_val, first_reg, OpType::Byte);
}

void cpu8086::SUB_R_IN_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = first_reg;
	

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		first_reg = first_reg - second_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg = first_reg - memory->readW(address);
	}
	testFlagZ(first_reg);
	testFlagS(first_reg, OpType::Word);
	testFlagP(first_reg);
	testFlagCSub(prev_val, first_reg);
	testFlagASub(prev_val, first_reg);
	testFlagO(prev_val, first_reg, OpType::Word);
}

void cpu8086::SUB_A_B() {
	byte data = fetchCodeByte();

	byte prev_val = A.low_high[0];
	
	A.low_high[0] = A.low_high[0] - data;

	testFlagZ(A.low_high[0]);
	testFlagS(A.low_high[0], OpType::Byte);
	testFlagP(A.low_high[0]);
	testFlagCSub(prev_val, A.low_high[0]);
	testFlagASub(prev_val, A.low_high[0]);
	testFlagO(prev_val, A.low_high[0], OpType::Byte);
}

void cpu8086::SUB_A_W() {
	word data = fetchCodeWord();

	word prev_val = A.X;
	
	A.X = A.X - data;

	testFlagZ(A.X);
	testFlagS(A.X, OpType::Word);
	testFlagP(A.X);
	testFlagCSub(prev_val, A.X);
	testFlagASub(prev_val, A.X);
	testFlagO(prev_val, A.X, OpType::Word);
}

void cpu8086::DAS() {
	byte prev_val = A.low_high[0];
	if ((A.low_high[0] & 0xF) > 9 || getFlag(Flag::A)) {
		A.low_high[0] -= 6;
		setFlag(Flag::A);
	}
	if (A.low_high[0] > 0x9F || getFlag(Flag::C)) {
		A.low_high[0] -= 0x60;
		setFlag(Flag::C);
	}
	testFlagZ(A.low_high[0]);
	testFlagS(A.low_high[0], OpType::Byte);
	testFlagP(A.low_high[0]);
	testFlagO(prev_val, A.low_high[0], OpType::Byte);
}

void cpu8086::XOR_R_OUT_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = 0;
	byte new_val = 0;
	

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		second_reg ^= first_reg;	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readB(address);
		new_val = prev_val ^ first_reg;
		memory->writeB(address, new_val);
	}
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Byte);
	testFlagP(new_val);
	remFlag(Flag::C);
	(new_val > 0) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::XOR_R_OUT_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = 0;
	word new_val = 0;
	

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		second_reg ^= first_reg;	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readW(address);
		new_val = prev_val ^ first_reg;
		memory->writeW(address, new_val);
	}
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Word);
	testFlagP(new_val);
	remFlag(Flag::C);
	(new_val > 0) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::XOR_R_IN_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);
	// определяем регистр для байтов
	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = first_reg;
	

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		first_reg ^= second_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg ^= memory->readB(address);
	}
	// установка флагов
	testFlagZ(first_reg);
	testFlagS(first_reg, OpType::Byte);
	testFlagP(first_reg);
	remFlag(Flag::C);
	(first_reg > 0) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::XOR_R_IN_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = first_reg;
	

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		first_reg ^= second_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg ^= memory->readW(address);
	}
	testFlagZ(first_reg);
	testFlagS(first_reg, OpType::Word);
	testFlagP(first_reg);
	remFlag(Flag::C);
	(first_reg > 0) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::XOR_A_B() {
	byte data = fetchCodeByte();

	byte prev_val = A.low_high[0];
	
	A.low_high[0] ^= data;

	testFlagZ(A.low_high[0]);
	testFlagS(A.low_high[0], OpType::Byte);
	testFlagP(A.low_high[0]);
	remFlag(Flag::C);
	(A.low_high[0] > 0) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::XOR_A_W() {
	word data = fetchCodeWord();

	word prev_val = A.X;
	
	A.X ^= data;

	testFlagZ(A.X);
	testFlagS(A.X, OpType::Word);
	testFlagP(A.X);
	remFlag(Flag::C);
	(A.X > 0) ? setFlag(Flag::A) : remFlag(Flag::A);
	remFlag(Flag::O);
}

void cpu8086::AAA() {
	byte prev_val = A.low_high[0];
	if ((A.low_high[0] & 0x0F) > 9 || getFlag(Flag::A)) {
		A.low_high[0] += 6;
		A.low_high[1] += 1;
		setFlag(Flag::A);
		setFlag(Flag::C);
	}
	A.low_high[0] = A.low_high[0] & 0x0F;
	testFlagZ(A.low_high[0]);
	testFlagS(A.low_high[0], OpType::Byte);
	testFlagP(A.low_high[0]);
	testFlagO(prev_val, A.low_high[0], OpType::Byte);
}

void cpu8086::CMP_R_OUT_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = 0;
	byte new_val = 0;
	

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		new_val = second_reg - first_reg;	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readB(address);
		new_val = prev_val - first_reg;
	}
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Byte);
	testFlagP(new_val);
	testFlagCSub(prev_val, new_val);
	testFlagASub(prev_val, new_val);
	testFlagO(prev_val, new_val, OpType::Byte);
}

void cpu8086::CMP_R_OUT_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = 0;
	word new_val = 0;
	

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		new_val = second_reg - first_reg;	// по идее такое невозможно
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readW(address);
		new_val = prev_val - first_reg;
	}
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Word);
	testFlagP(new_val);
	testFlagCSub(prev_val, new_val);
	testFlagASub(prev_val, new_val);
	testFlagO(prev_val, new_val, OpType::Word);
}

void cpu8086::CMP_R_IN_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);
	// определяем регистр для байтов
	byte& first_reg = getRegB(reg);

	// переменные нужны для проверки флагов
	byte prev_val = first_reg;
	byte new_val = 0;
	

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		new_val = first_reg - second_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		new_val = first_reg - memory->readB(address);
	}
	// установка флагов
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Byte);
	testFlagP(new_val);
	testFlagCSub(prev_val, new_val);
	testFlagASub(prev_val, new_val);
	testFlagO(prev_val, new_val, OpType::Byte);
}

void cpu8086::CMP_R_IN_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	// переменные нужны для проверки флагов
	word prev_val = first_reg;
	word new_val = 0;	// временная переменная с которой выполняется сравнение

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		new_val = first_reg - second_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		new_val = first_reg - memory->readW(address);
	}
	testFlagZ(new_val);
	testFlagS(new_val, OpType::Word);
	testFlagP(new_val);
	testFlagCSub(prev_val, new_val);
	testFlagASub(prev_val, new_val);
	testFlagO(prev_val, new_val, OpType::Word);
}

void cpu8086::CMP_A_B() {
	byte data = fetchCodeByte();
	byte temp = 0;	// временная переменная с которой выполняется сравнение

	byte prev_val = A.low_high[0];
	
	temp = A.low_high[0] - data;

	testFlagZ(temp);
	testFlagS(temp, OpType::Byte);
	testFlagP(temp);
	testFlagCSub(prev_val, temp);
	testFlagASub(prev_val, temp);
	testFlagO(prev_val, temp, OpType::Byte);
}

void cpu8086::CMP_A_W() {
	word data = fetchCodeWord();
	word temp = 0;	// временная переменная с которой выполняется сравнение

	word prev_val = A.X;
	
	temp = A.X - data;

	testFlagZ(temp);
	testFlagS(temp, OpType::Word);
	testFlagP(temp);
	testFlagCSub(prev_val, temp);
	testFlagASub(prev_val, temp);
	testFlagO(prev_val, temp, OpType::Word);
}

void cpu8086::AAS() {
	byte prev_val = A.low_high[0];
	if ((A.low_high[0] & 0x0F) > 9 || getFlag(Flag::A)) {
		A.low_high[0] -= 6;
		A.low_high[1] -= 1;
		setFlag(Flag::A);
		setFlag(Flag::C);
	}
	A.low_high[0] = A.low_high[0] & 0x0F;
	testFlagZ(A.low_high[0]);
	testFlagS(A.low_high[0], OpType::Byte);
	testFlagP(A.low_high[0]);
	testFlagO(prev_val, A.low_high[0], OpType::Byte);
}

void cpu8086::INC_R(word& reg) {
	word prev_val = reg;
	reg++;
	// affected flags
	testFlagZ(reg);
	testFlagS(reg, OpType::Word);
	testFlagP(reg);
	testFlagAAdd(prev_val, reg);
	testFlagO(prev_val, reg, OpType::Word);
}

void cpu8086::DEC_R(word& reg) {
	word prev_val = reg;
	reg--;
	// affected flags
	testFlagZ(reg);
	testFlagS(reg, OpType::Word);
	testFlagP(reg);
	testFlagASub(prev_val, reg);
	testFlagO(prev_val, reg, OpType::Word);
}

void cpu8086::PUSH_R(word& reg) {
	SP -= 2;	// стек растёт вниз
	address = ((dword)SS << 4) + SP;	// получение физического адреса вершины стека
	memory->writeW(address, reg);	
}

void cpu8086::POP_R(word& reg) {
	address = ((dword)SS << 4) + SP;
	reg = memory->readW(address);
	SP += 2;
}

void cpu8086::JMP_COND(std::function<bool()> condition) {
	// приводим к типу со знаком чтобы можно было вычитать
	char short_label = std::make_signed_t<char>(fetchCodeByte());
	if (condition()) {	// проверка условия
		IP += short_label;	// условный переход
	}
}

void cpu8086::IMMED_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	byte& first_reg = getRegB(rm);

	// переменные нужны для проверки флагов
	byte prev_val = 0;
	byte new_val = 0;
	byte data = 0;	// константа, которая идёт после команды

	if (mod == 3) {	// регистровая адресация
		data = fetchCodeByte();
		prev_val = first_reg;
		switch (reg) {
		case 0:		// ADD
			first_reg = first_reg + data;
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Byte);
			testFlagP(first_reg);
			testFlagCAdd(prev_val, first_reg);
			testFlagAAdd(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Byte);
			break;
		case 1:		// OR
			first_reg = first_reg | data;
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Byte);
			testFlagP(first_reg);
			remFlag(Flag::C);
			(first_reg > prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
			remFlag(Flag::O);
			break;
		case 2:		// ADC
			first_reg = first_reg + data + getFlag(Flag::C);
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Byte);
			testFlagP(first_reg);
			testFlagCAdd(prev_val, first_reg);
			testFlagAAdd(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Byte);
			break;
		case 3:		// SBB
			first_reg = first_reg - data - getFlag(Flag::C);
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Byte);
			testFlagP(first_reg);
			testFlagCSub(prev_val, first_reg);
			testFlagASub(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Byte);
			break;
		case 4:		// AND
			first_reg = first_reg & data;
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Byte);
			testFlagP(first_reg);
			remFlag(Flag::C);
			(first_reg & prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
			remFlag(Flag::O);
			break;
		case 5:		// SUB
			first_reg = first_reg - data;
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Byte);
			testFlagP(first_reg);
			testFlagCSub(prev_val, first_reg);
			testFlagASub(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Byte);
			break;
		case 6:		// XOR
			first_reg = first_reg ^ data;
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Byte);
			testFlagP(first_reg);
			remFlag(Flag::C);
			(first_reg > 0) ? setFlag(Flag::A) : remFlag(Flag::A);
			remFlag(Flag::O);
			break;
		case 7: {		// CMP
			byte temp = first_reg;
			temp = first_reg - data;
			testFlagZ(temp);
			testFlagS(temp, OpType::Byte);
			testFlagP(temp);
			testFlagCSub(prev_val, temp);
			testFlagASub(prev_val, temp);
			testFlagO(prev_val, temp, OpType::Byte);
			break;
		}
		default: return;
		}
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readB(address);
		data = fetchCodeByte();

		switch (reg) {
		case 0:		// ADD
			new_val = prev_val + data;
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Byte);
			testFlagP(new_val);
			testFlagCAdd(prev_val, new_val);
			testFlagAAdd(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Byte);
			break;
		case 1:		// OR
			new_val = prev_val | data;
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Byte);
			testFlagP(new_val);
			remFlag(Flag::C);
			(new_val > prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
			remFlag(Flag::O);
			break;
		case 2:		// ADC
			new_val = prev_val + data + getFlag(Flag::C);
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Byte);
			testFlagP(new_val);
			testFlagCAdd(prev_val, new_val);
			testFlagAAdd(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Byte);
			break;
		case 3:		// SBB
			new_val = prev_val - data - getFlag(Flag::C);
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Byte);
			testFlagP(new_val);
			testFlagCSub(prev_val, new_val);
			testFlagASub(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Byte);
			break;
		case 4:		// AND
			new_val = prev_val & data;
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Byte);
			testFlagP(new_val);
			remFlag(Flag::C);
			(new_val & prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
			remFlag(Flag::O);
			break;
		case 5:		// SUB
			new_val = prev_val - data;
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Byte);
			testFlagP(new_val);
			testFlagCSub(prev_val, new_val);
			testFlagASub(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Byte);
			break;
		case 6:		// XOR
			new_val = prev_val ^ data;
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Byte);
			testFlagP(new_val);
			remFlag(Flag::C);
			(new_val > 0) ? setFlag(Flag::A) : remFlag(Flag::A);
			remFlag(Flag::O);
			break;
		case 7: {	// CMP
			byte temp = first_reg;
			new_val = prev_val;	// что бы не менять значение в памяти
			temp = prev_val - data;
			testFlagZ(temp);
			testFlagS(temp, OpType::Byte);
			testFlagP(temp);
			testFlagCSub(prev_val, temp);
			testFlagASub(prev_val, temp);
			testFlagO(prev_val, temp, OpType::Byte);
			break;
		}
		default: return;
		}

		memory->writeB(address, new_val);
	}
}

void cpu8086::IMMED_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(rm);

	// переменные нужны для проверки флагов
	word prev_val = 0;
	word new_val = 0;
	word data = 0;	// константа, которая идёт после команды

	if (mod == 3) {	// регистровая адресация
		data = fetchCodeWord();
		prev_val = first_reg;
		switch (reg) {
		case 0:		// ADD
			first_reg = first_reg + data;
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Word);
			testFlagP(first_reg);
			testFlagCAdd(prev_val, first_reg);
			testFlagAAdd(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Word);
			break;
		case 1:		// OR
			first_reg = first_reg | data;
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Word);
			testFlagP(first_reg);
			remFlag(Flag::C);
			(first_reg > prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
			remFlag(Flag::O);
			break;
		case 2:		// ADC
			first_reg = first_reg + data + getFlag(Flag::C);
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Word);
			testFlagP(first_reg);
			testFlagCAdd(prev_val, first_reg);
			testFlagAAdd(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Word);
			break;
		case 3:		// SBB
			first_reg = first_reg - data - getFlag(Flag::C);
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Word);
			testFlagP(first_reg);
			testFlagCSub(prev_val, first_reg);
			testFlagASub(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Word);
			break;
		case 4:		// AND
			first_reg = first_reg & data;
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Word);
			testFlagP(first_reg);
			remFlag(Flag::C);
			(first_reg & prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
			remFlag(Flag::O);
			break;
		case 5:		// SUB
			first_reg = first_reg - data;
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Word);
			testFlagP(first_reg);
			testFlagCSub(prev_val, first_reg);
			testFlagASub(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Word);
			break;
		case 6:		// XOR
			first_reg = first_reg ^ data;
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Word);
			testFlagP(first_reg);
			remFlag(Flag::C);
			(first_reg > 0) ? setFlag(Flag::A) : remFlag(Flag::A);
			remFlag(Flag::O);
			break;
		case 7: {		// CMP
			word temp = first_reg;
			temp = first_reg - data;
			testFlagZ(temp);
			testFlagS(temp, OpType::Word);
			testFlagP(temp);
			testFlagCSub(prev_val, temp);
			testFlagASub(prev_val, temp);
			testFlagO(prev_val, temp, OpType::Word);
			break;
		}
		default: return;
		}
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readW(address);
		data = fetchCodeWord();

		switch (reg) {
		case 0:		// ADD
			new_val = prev_val + data;
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Word);
			testFlagP(new_val);
			testFlagCAdd(prev_val, new_val);
			testFlagAAdd(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Word);
			break;
		case 1:		// OR
			new_val = prev_val | data;
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Word);
			testFlagP(new_val);
			remFlag(Flag::C);
			(new_val > prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
			remFlag(Flag::O);
			break;
		case 2:		// ADC
			new_val = prev_val + data + getFlag(Flag::C);
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Word);
			testFlagP(new_val);
			testFlagCAdd(prev_val, new_val);
			testFlagAAdd(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Word);
			break;
		case 3:		// SBB
			new_val = prev_val - data - getFlag(Flag::C);
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Word);
			testFlagP(new_val);
			testFlagCSub(prev_val, new_val);
			testFlagASub(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Word);
			break;
		case 4:		// AND
			new_val = prev_val & data;
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Word);
			testFlagP(new_val);
			remFlag(Flag::C);
			(new_val & prev_val) ? setFlag(Flag::A) : remFlag(Flag::A);
			remFlag(Flag::O);
			break;
		case 5:		// SUB
			new_val = prev_val - data;
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Word);
			testFlagP(new_val);
			testFlagCSub(prev_val, new_val);
			testFlagASub(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Word);
			break;
		case 6:		// XOR
			new_val = prev_val ^ data;
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Word);
			testFlagP(new_val);
			remFlag(Flag::C);
			(new_val > 0) ? setFlag(Flag::A) : remFlag(Flag::A);
			remFlag(Flag::O);
			break;
		case 7: {	// CMP
			word temp = first_reg;
			new_val = prev_val;	// что бы не менять значение в памяти
			temp = prev_val - data;
			testFlagZ(temp);
			testFlagS(temp, OpType::Word);
			testFlagP(temp);
			testFlagCSub(prev_val, temp);
			testFlagASub(prev_val, temp);
			testFlagO(prev_val, temp, OpType::Word);
			break;
		}
		default: return;
		}

		memory->writeW(address, new_val);
	}
}

void cpu8086::IMMED_B_SX() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	byte& first_reg = getRegB(rm);

	// переменные нужны для проверки флагов
	byte prev_val = 0;
	byte new_val = 0;
	byte data = 0;	// константа, которая идёт после команды

	if (mod == 3) {	// регистровая адресация
		data = fetchCodeByte();
		prev_val = first_reg;
		switch (reg) {
		case 0:		// ADD
			first_reg = first_reg + data;
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Byte);
			testFlagP(first_reg);
			testFlagCAdd(prev_val, first_reg);
			testFlagAAdd(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Byte);
			break;
		case 1:		// not used
			return;
		case 2:		// ADC
			first_reg = first_reg + data + getFlag(Flag::C);
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Byte);
			testFlagP(first_reg);
			testFlagCAdd(prev_val, first_reg);
			testFlagAAdd(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Byte);
			break;
		case 3:		// SBB
			first_reg = first_reg - data - getFlag(Flag::C);
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Byte);
			testFlagP(first_reg);
			testFlagCSub(prev_val, first_reg);
			testFlagASub(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Byte);
			break;
		case 4:		// not used
			return;
		case 5:		// SUB
			first_reg = first_reg - data;
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Byte);
			testFlagP(first_reg);
			testFlagCSub(prev_val, first_reg);
			testFlagASub(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Byte);
			break;
		case 6:		// not used
			return;
		case 7: {		// CMP
			byte temp = first_reg;
			temp = first_reg - data;
			testFlagZ(temp);
			testFlagS(temp, OpType::Byte);
			testFlagP(temp);
			testFlagCSub(prev_val, temp);
			testFlagASub(prev_val, temp);
			testFlagO(prev_val, temp, OpType::Byte);
			break;
		}
		default: return;
		}
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readB(address);
		data = fetchCodeByte();

		switch (reg) {
		case 0:		// ADD
			new_val = prev_val + data;
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Byte);
			testFlagP(new_val);
			testFlagCAdd(prev_val, new_val);
			testFlagAAdd(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Byte);
			break;
		case 2:		// ADC
			new_val = prev_val + data + getFlag(Flag::C);
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Byte);
			testFlagP(new_val);
			testFlagCAdd(prev_val, new_val);
			testFlagAAdd(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Byte);
			break;
		case 3:		// SBB
			new_val = prev_val - data - getFlag(Flag::C);
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Byte);
			testFlagP(new_val);
			testFlagCSub(prev_val, new_val);
			testFlagASub(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Byte);
			break;
		case 5:		// SUB
			new_val = prev_val - data;
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Byte);
			testFlagP(new_val);
			testFlagCSub(prev_val, new_val);
			testFlagASub(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Byte);
			break;
		case 7: {	// CMP
			byte temp = first_reg;
			new_val = prev_val;	// что бы не менять значение в памяти
			temp = prev_val - data;
			testFlagZ(temp);
			testFlagS(temp, OpType::Byte);
			testFlagP(temp);
			testFlagCSub(prev_val, temp);
			testFlagASub(prev_val, temp);
			testFlagO(prev_val, temp, OpType::Byte);
			break;
		}
		default: return;
		}

		memory->writeB(address, new_val);
	}
}

void cpu8086::IMMED_W_SX() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(rm);

	// переменные нужны для проверки флагов
	word prev_val = 0;
	word new_val = 0;
	char data = 0;	// константа, которая идёт после команды

	if (mod == 3) {	// регистровая адресация
		data = std::make_signed_t<char>(fetchCodeByte());

		prev_val = first_reg;
		switch (reg) {
		case 0:		// ADD
			first_reg = first_reg + data;
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Word);
			testFlagP(first_reg);
			testFlagCAdd(prev_val, first_reg);
			testFlagAAdd(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Word);
			break;
		case 2:		// ADC
			first_reg = first_reg + data + getFlag(Flag::C);
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Word);
			testFlagP(first_reg);
			testFlagCAdd(prev_val, first_reg);
			testFlagAAdd(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Word);
			break;
		case 3:		// SBB
			first_reg = first_reg - data - getFlag(Flag::C);
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Word);
			testFlagP(first_reg);
			testFlagCSub(prev_val, first_reg);
			testFlagASub(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Word);
			break;
		case 5:		// SUB
			first_reg = first_reg - data;
			testFlagZ(first_reg);
			testFlagS(first_reg, OpType::Word);
			testFlagP(first_reg);
			testFlagCSub(prev_val, first_reg);
			testFlagASub(prev_val, first_reg);
			testFlagO(prev_val, first_reg, OpType::Word);
			break;
		case 7: {		// CMP
			word temp = first_reg;
			temp = first_reg - data;
			testFlagZ(temp);
			testFlagS(temp, OpType::Word);
			testFlagP(temp);
			testFlagCSub(prev_val, temp);
			testFlagASub(prev_val, temp);
			testFlagO(prev_val, temp, OpType::Word);
			break;
		}
		default: return;
		}
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		prev_val = memory->readW(address);
		data = std::make_signed_t<char>(fetchCodeByte());

		switch (reg) {
		case 0:		// ADD
			new_val = prev_val + data;
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Word);
			testFlagP(new_val);
			testFlagCAdd(prev_val, new_val);
			testFlagAAdd(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Word);
			break;
		case 2:		// ADC
			new_val = prev_val + data + getFlag(Flag::C);
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Word);
			testFlagP(new_val);
			testFlagCAdd(prev_val, new_val);
			testFlagAAdd(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Word);
			break;
		case 3:		// SBB
			new_val = prev_val - data - getFlag(Flag::C);
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Word);
			testFlagP(new_val);
			testFlagCSub(prev_val, new_val);
			testFlagASub(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Word);
			break;
		case 5:		// SUB
			new_val = prev_val - data;
			testFlagZ(new_val);
			testFlagS(new_val, OpType::Word);
			testFlagP(new_val);
			testFlagCSub(prev_val, new_val);
			testFlagASub(prev_val, new_val);
			testFlagO(prev_val, new_val, OpType::Word);
			break;
		case 7: {	// CMP
			word temp = first_reg;
			new_val = prev_val;	// что бы не менять значение в памяти
			temp = prev_val - data;
			testFlagZ(temp);
			testFlagS(temp, OpType::Word);
			testFlagP(temp);
			testFlagCSub(prev_val, temp);
			testFlagASub(prev_val, temp);
			testFlagO(prev_val, temp, OpType::Word);
			break;
		}
		default: return;
		}

		memory->writeW(address, new_val);
	}
}

void cpu8086::MOV_R_IMM_B(byte& reg) {
	reg = fetchCodeByte();
}

void cpu8086::MOV_R_IMM_W(word& reg) {
	reg = fetchCodeWord();
}

void cpu8086::MOV_A_IN_B() {
	address = fetchCodeWord();
	A.low_high[0] = memory->readB(((dword)DS << 4) + address);
}

void cpu8086::MOV_A_IN_W() {
	address = fetchCodeWord();
	A.X = memory->readW(((dword)DS << 4) + address);
}

void cpu8086::MOV_A_OUT_B() {
	address = fetchCodeWord();
	memory->writeB(((dword)DS << 4) + address, A.low_high[0]);
}

void cpu8086::MOV_A_OUT_W() {
	address = fetchCodeWord();
	memory->writeW(((dword)DS << 4) + address, A.X);
}

void cpu8086::XCHG_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	byte& first_reg = getRegB(reg);

	byte temp = 0;

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		temp = first_reg;
		first_reg = second_reg;
		second_reg = temp;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		temp = memory->readB(address);
		memory->writeB(address, first_reg);
		first_reg = temp;
	}
}

void cpu8086::XCHG_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	word temp = 0;

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		temp = first_reg;
		first_reg = second_reg;
		second_reg = temp;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение

		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		temp = memory->readW(address);
		memory->writeW(address, first_reg);
		first_reg = temp;
	}
}

void cpu8086::MOV_MEM_IMM_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	// остальные команды не используются
	if (reg != 0) return;

	// получаем смещение
	word displacement = fetchDisp(mod, rm);
	// получаем эффективный адрес
	word EA = fetchEA(mod, rm, displacement);
	address = ((dword)DS << 4) + EA;
	// получаем прямое значение после получения адреса и записываем его
	memory->writeB(address, fetchCodeByte());
}

void cpu8086::MOV_MEM_IMM_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	// остальные команды не используются
	if (reg != 0) return;

	// получаем смещение
	word displacement = fetchDisp(mod, rm);
	// получаем эффективный адрес
	word EA = fetchEA(mod, rm, displacement);
	address = ((dword)DS << 4) + EA;
	memory->writeW(address, fetchCodeWord());
}

void cpu8086::MOV_R_OUT_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	byte& first_reg = getRegB(reg);

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		second_reg = first_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение
		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		memory->writeB(address, first_reg);
	}
}

void cpu8086::MOV_R_OUT_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		second_reg = first_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение
		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		memory->writeW(address, first_reg);
	}
}

void cpu8086::MOV_R_IN_B() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	byte& first_reg = getRegB(reg);

	if (mod == 3) {	// регистровая адресация
		byte& second_reg = getRegB(rm);
		first_reg = second_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение
		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg = memory->readB(address);
	}
}

void cpu8086::MOV_R_IN_W() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	word& first_reg = getRegW(reg);

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		first_reg = second_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение
		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg = memory->readW(address);
	}
}

void cpu8086::MOV_SR_OUT() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	// остальные не используются
	if (reg >> 3 != 0) return;

	word& first_reg = getSegReg(reg);

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		second_reg = first_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение
		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		memory->writeW(address, first_reg);
	}
}

void cpu8086::MOV_SR_IN() {
	byte mod, reg, rm;
	fetchModRegRm(mod, reg, rm);

	// остальные не используются
	if (reg >> 3 != 0) return;

	word& first_reg = getSegReg(reg);

	if (mod == 3) {	// регистровая адресация
		word& second_reg = getRegW(rm);
		first_reg = second_reg;
	}
	else {	// вычисление эффективного адреса
		word displacement = fetchDisp(mod, rm);	// смещение
		// получаем эффективный адрес
		word EA = fetchEA(mod, rm, displacement);
		address = ((dword)DS << 4) + EA;
		first_reg = memory->readW(address);
	}
}

void cpu8086::HLT() {
	presenter->notifyHalt();
}
/******OPCODES_END******/