#pragma once


namespace GraphConst {
	const int base_border = 5;
	const int flag_field_width = 15;
	const int flag_field_height = 20;
	const int byte_field_width = 30;
	const int byte_field_height = 20;
	const int word_field_width = 40;
	const int word_field_height = byte_field_height;
	const int memory_cols = 16;
	const int memory_rows = 16;
	const int row_label_size = 40;
	const int col_label_size = 16;


	const int screen_width = 800;
	const int screen_height = 600;



	enum ButtonsIDs {
		CLOCK, RUN, STOP, LOAD
	};

	enum FieldIDs {
		START_ADDRESS,
		CODE_FIELD,
		AL, AH,
		BL, BH,
		CL, CH,
		DL, DH,
		SP, BP, SI, DI,
		CS, DS, SS, ES,
		IP,
		O, S, Z, P, C, A
	};
};