#include <stdlib.h>
#include "b_vm.h"

struct b_vm_t {
	int version;
	int maxStack;
};

struct b_vm_t* b_vm_new_default() {
	struct b_vm_t* self = (struct b_vm_t*) calloc(1, sizeof(struct b_vm_t));
	if (!self) {
		return 0;
	}
	self->maxStack = 1024;
	return self;
}

static unsigned int b_vm_read_index(unsigned char** bytes) {
	unsigned int i1 = *((*bytes)++);
	if (i1 <= 0x7F) {
		return i1;
	} else if ((i1 >> 5) == 0x6) {
		int i2 = *((*bytes)++);
		return ((i1 & 0x1F) << 6) + (i2 & 0x3F);
	} else if ((i1 >> 4) == 0xE) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		return ((i1 & 0xF) << 12) + ((i2 & 0x3F) << 6) + (i3 & 0x3F);
	} else if ((i1 >> 3) == 0x1E) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		return ((i1 & 0x7) << 18) + ((i2 & 0x3F) << 12) + ((i3 & 0x3F) << 6)
				+ (i4 & 0x3F);
	} else if ((i1 >> 2) == 0x3E) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		int i5 = *((*bytes)++);
		return ((i1 & 0x3) << 24) + ((i2 & 0x3F) << 18) + ((i3 & 0x3F) << 12)
				+ ((i4 & 0x3F) << 6) + (i5 & 0x3F);
	} else if ((i1 >> 1) == 0x7E) {
		int i2 = *((*bytes)++);
		int i3 = *((*bytes)++);
		int i4 = *((*bytes)++);
		int i5 = *((*bytes)++);
		int i6 = *((*bytes)++);
		return ((i1 & 0x1) << 30) + ((i2 & 0x3F) << 24) + ((i3 & 0x3F) << 18)
				+ ((i4 & 0x3F) << 12) + ((i5 & 0x3F) << 6) + (i6 & 0x3F);
	} else {
		return 0;
	}
}

unsigned int b_vm_execute(unsigned char* opcodes, int length, void* result,
		void* exception) {

	switch (*opcodes++) {
	case B_VM_OPCODE_STACK_INC: {
		unsigned int index = b_vm_read_index(&opcodes);
		break;
	}
	case B_VM_OPCODE_STACK_DEC: {
		unsigned int index = b_vm_read_index(&opcodes);
		break;
	}
	case B_VM_OPCODE_STACK_STRING: {
		break;
	}
	case B_VM_OPCODE_STACK_DOUBLE: {
		break;
	}
	case B_VM_OPCODE_STACK_INTEGER: {
		break;
	}
	case B_VM_OPCODE_STACK_TRUE: {
		break;
	}
	case B_VM_OPCODE_STACK_FALSE: {
		break;
	}
	case B_VM_OPCODE_STACK_NULL: {
		break;
	}
	case B_VM_OPCODE_NUMBER_SUM: {
		break;
	}
	case B_VM_OPCODE_NUMBER_SUB: {
		break;
	}
	case B_VM_OPCODE_NUMBER_MUL: {
		break;
	}
	case B_VM_OPCODE_NUMBER_DIV: {
		break;
	}
	case B_VM_OPCODE_NUMBER_EQ: {
		break;
	}
	case B_VM_OPCODE_NUMBER_NEQ: {
		break;
	}
	case B_VM_OPCODE_NUMBER_COMPARE: {
		break;
	}
	case B_VM_OPCODE_NUMBER_GT: {
		break;
	}
	case B_VM_OPCODE_NUMBER_GE: {
		break;
	}
	case B_VM_OPCODE_NUMBER_LT: {
		break;
	}
	case B_VM_OPCODE_NUMBER_LE: {
		break;
	}
	case B_VM_OPCODE_NUMBER_MOD: {
		break;
	}
	case B_VM_OPCODE_NUMBER_INT_DIV: {
		break;
	}
	case B_VM_OPCODE_NUMBER_TO_STRING: {
		break;
	}
	case B_VM_OPCODE_NUMBER_IS_NAN: {
		break;
	}
	case B_VM_OPCODE_NUMBER_IS_INFINITY: {
		break;
	}
	case B_VM_OPCODE_NUMBER_HASH: {
		break;
	}
	case B_VM_OPCODE_NUMBER_NEG: {
		break;
	}
	case B_VM_OPCODE_NUMBER_INC: {
		break;
	}
	case B_VM_OPCODE_NUMBER_DEC: {
		break;
	}
	case B_VM_OPCODE_INTEGER_SUM: {
		break;
	}
	case B_VM_OPCODE_INTEGER_SUB: {
		break;
	}
	case B_VM_OPCODE_INTEGER_MUL: {
		break;
	}
	case B_VM_OPCODE_INTEGER_DIV: {
		break;
	}
	case B_VM_OPCODE_INTEGER_EQ: {
		break;
	}
	case B_VM_OPCODE_INTEGER_NEQ: {
		break;
	}
	case B_VM_OPCODE_INTEGER_COMPARE: {
		break;
	}
	case B_VM_OPCODE_INTEGER_GT: {
		break;
	}
	case B_VM_OPCODE_INTEGER_GE: {
		break;
	}
	case B_VM_OPCODE_INTEGER_LT: {
		break;
	}
	case B_VM_OPCODE_INTEGER_LE: {
		break;
	}
	case B_VM_OPCODE_INTEGER_AND: {
		break;
	}
	case B_VM_OPCODE_INTEGER_OR: {
		break;
	}
	case B_VM_OPCODE_INTEGER_MOD: {
		break;
	}
	case B_VM_OPCODE_INTEGER_TO_STRING: {
		break;
	}
	case B_VM_OPCODE_INTEGER_HASH: {
		break;
	}
	case B_VM_OPCODE_INTEGER_NEG: {
		break;
	}
	case B_VM_OPCODE_INTEGER_INC: {
		break;
	}
	case B_VM_OPCODE_INTEGER_DEC: {
		break;
	}
	case B_VM_OPCODE_BOOLEAN_NOT: {
		break;
	}
	case B_VM_OPCODE_BOOLEAN_AND: {
		break;
	}
	case B_VM_OPCODE_BOOLEAN_OR: {
		break;
	}
	case B_VM_OPCODE_STRING_SUM: {
		break;
	}
	case B_VM_OPCODE_CONTROL_JUMP: {
		break;
	}
	case B_VM_OPCODE_CONTROL_JUMP_TRUE: {
		break;
	}
	case B_VM_OPCODE_CONTROL_JUMP_FALSE: {
		break;
	}
	case B_VM_OPCODE_CONTROL_JUMP_INT: {
		break;
	}
	case B_VM_OPCODE_CONTROL_RETURN: {
		break;
	}
	case B_VM_OPCODE_HALF: {
		break;
	}
	default: {
		return 1;
	}
	}
	return 0;
}
