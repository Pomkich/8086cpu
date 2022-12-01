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
	opcode = ((int)CS << 4) + IP;	// получение физического адреса
	opcode_table[opcode]();			// выполнение команды
	IP++;							// установка счётчика на следующую команду
}

// фкнкции проверки флагов
void cpu8086::testFlagZ(word& src_op) {
	(src_op == 0) ? setFlag(Flag::Z) : remFlag(Flag::Z);
}

void cpu8086::testFlagS(word& src_op) {
	((src_op >> 15) & 1) ? setFlag(Flag::S) : remFlag(Flag::S);
}

void cpu8086::testFlagS(byte& src_op) {
	((src_op >> 7) & 1) ? setFlag(Flag::S) : remFlag(Flag::S);
}

void cpu8086::testFlagP(byte val) {
	// counting setted bits
	byte set_bits = 0;
	for (int i = 0; i < 8; i++) {
		if ((val & 1) == 1) set_bits++;
		val = val >> 1;
	}
	if (set_bits % 2 == 0) setFlag(Flag::P);	// parity bit set
	else remFlag(Flag::P);
}

void cpu8086::testFlagCAdd(byte prev_val, byte& src_op) {
	if (prev_val > src_op) setFlag(Flag::C);	// carry bit set add
	else remFlag(Flag::C);
}

void cpu8086::testFlagCSub(byte prev_val, byte& src_op) {
	if (prev_val < src_op) setFlag(Flag::C);	// carry bit set sub
	else remFlag(Flag::C);
}

void cpu8086::testFlagCAdd(word prev_val, word& src_op) {
	if (prev_val > src_op) setFlag(Flag::C);	// carry bit set add
	else remFlag(Flag::C);
}

void cpu8086::testFlagCSub(word prev_val, word& src_op) {
	if (prev_val < src_op) setFlag(Flag::C);	// carry bit set sub
	else remFlag(Flag::C);
}

void cpu8086::testFlagAAdd(word prev_val, word& src_op) {
	if ((prev_val & 0x000F) > (src_op & 0x000F)) setFlag(Flag::A);	// Auxiliary carry bit set add
	else remFlag(Flag::A);
}

void cpu8086::testFlagASub(word prev_val, word& src_op) {
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

// функция инициализирует таблицу команд
void cpu8086::initOpTable() {
	opcode_table[0x40] = std::bind(&cpu8086::INC_R, this, std::ref(A.X));
	opcode_table[0x41] = std::bind(&cpu8086::INC_R, this, std::ref(B.X));
	opcode_table[0x42] = std::bind(&cpu8086::INC_R, this, std::ref(C.X));
	opcode_table[0x43] = std::bind(&cpu8086::INC_R, this, std::ref(D.X));
	opcode_table[0x44] = std::bind(&cpu8086::INC_R, this, std::ref(SP));
	opcode_table[0x45] = std::bind(&cpu8086::INC_R, this, std::ref(BP));
	opcode_table[0x46] = std::bind(&cpu8086::INC_R, this, std::ref(SI));
	opcode_table[0x47] = std::bind(&cpu8086::INC_R, this, std::ref(DI));
}

/******OPCODES_BEG******/ 
void cpu8086::INC_R(word& rgs) {
	word prev_val = rgs;
	rgs++;
	// affected flags
	testFlagZ(rgs);
	testFlagS(rgs);
	testFlagP(rgs);
	testFlagAAdd(prev_val, rgs);
}
/******OPCODES_END******/