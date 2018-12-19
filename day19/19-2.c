#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define REGISTER_COUNT 6
#define OPCODE_COUNT 16
#define OPCODE_LEN 5
#define MAX_INSTRUCTIONS 100

typedef int Registers[REGISTER_COUNT];

typedef enum {
    OPCODE_ADDR,
    OPCODE_ADDI,
    OPCODE_MULR,
    OPCODE_MULI,
    OPCODE_BANR,
    OPCODE_BANI,
    OPCODE_BORR,
    OPCODE_BORI,
    OPCODE_SETR,
    OPCODE_SETI,
    OPCODE_GTIR,
    OPCODE_GTRI,
    OPCODE_GTRR,
    OPCODE_EQIR,
    OPCODE_EQRI,
    OPCODE_EQRR,
} Opcode;

const char opcode_strings[][OPCODE_LEN] = {
    "addr",
    "addi",
    "mulr",
    "muli",
    "banr",
    "bani",
    "borr",
    "bori",
    "setr",
    "seti",
    "gtir",
    "gtri",
    "gtrr",
    "eqir",
    "eqri",
    "eqrr",
};

typedef struct {
    Opcode opcode;
    int input_a;
    int input_b;
    int output_reg;
} Instruction;

typedef struct {
    int ip;
    int ip_reg;
    Registers registers;
    Instruction instructions[MAX_INSTRUCTIONS];
    int instruction_count;
} Program;

void init_program(Program *p) {
    p->ip = 11;
    p->ip_reg = 0;
    
    p->registers[0] = 0;
    p->registers[1] = 11;
    p->registers[2] = 10551200;
    p->registers[3] = 0;
    p->registers[4] = 10551261;
    p->registers[5] = 1;
    
    p->instruction_count = 0;
}

Opcode str_to_opcode(char *str) {
    for(int i = 0; i < OPCODE_COUNT; i++) {
        if(strcmp(str, opcode_strings[i]) == 0) {
            return i;
        }
    }
    
    return 0;
}

void read_program(Program *p, char *filename) {
    FILE *fp = fopen(filename, "r");
    
    fscanf(fp, "#ip %d\n", &p->ip_reg);
    
    char opcode[OPCODE_LEN];
    int input_a;
    int input_b;
    int output_reg;
    
    while(fscanf(fp, "%s %d %d %d\n", opcode, &input_a, &input_b, &output_reg) == 4) {
        Instruction *i = &p->instructions[p->instruction_count];
        i->opcode = str_to_opcode(opcode);
        i->input_a = input_a;
        i->input_b = input_b;
        i->output_reg = output_reg;
        
        p->instruction_count++;
    }
    
    fclose(fp);
}

void print_instruction(Instruction *instruction) {
    printf("%s %d %d %d", opcode_strings[instruction->opcode], instruction->input_a, instruction->input_b, instruction->output_reg);
}

void print_program(Program *p) {
    printf("IP Register: %d\n", p->ip_reg);
    
    for(int i = 0; i < p->instruction_count; i++) {
        printf("Instruction %d: ", i);
        print_instruction(&p->instructions[i]);
        printf("\n");
    }
}

bool is_ip_valid(Program *p) {
    return (p->ip >= 0) && (p->ip < p->instruction_count);
}

void print_registers(Registers r) {
    printf("[");
    
    for(int i = 0; i < REGISTER_COUNT; i++) {
        printf("%d", r[i]);
        
        if(i != REGISTER_COUNT - 1) {
            printf(", ");
        }
    }
    
    printf("]");
}

void execute_instruction(Program *p) {
    //printf("ip=%d ", p->ip);
    p->registers[p->ip_reg] = p->ip;
    
    //print_registers(p->registers);
    //printf(" ");
    
    Instruction *instruction = &p->instructions[p->ip];
    //print_instruction(instruction);
    //printf(" ");
    
    
    
    int value_a = instruction->input_a;
    int value_b = instruction->input_b;
    
    int register_a = p->registers[value_a];
    int register_b = p->registers[value_b];
    
    int *output_reg = &p->registers[instruction->output_reg];
    
    if(instruction->opcode == OPCODE_ADDR) {
        *output_reg = register_a + register_b;
    } else if(instruction->opcode == OPCODE_ADDI) {
        *output_reg = register_a + value_b;
    } else if(instruction->opcode == OPCODE_MULR) {
        *output_reg = register_a * register_b;
    } else if(instruction->opcode == OPCODE_MULI) {
        *output_reg = register_a * value_b;
    } else if(instruction->opcode == OPCODE_BANR) {
        *output_reg = register_a & register_b;
    } else if(instruction->opcode == OPCODE_BANI) {
        *output_reg = register_a & value_b;
    } else if(instruction->opcode == OPCODE_BORR) {
        *output_reg = register_a | register_b;
    } else if(instruction->opcode == OPCODE_BORI) {
        *output_reg = register_a | value_b;
    } else if(instruction->opcode == OPCODE_SETR) {
        *output_reg = register_a;
    } else if(instruction->opcode == OPCODE_SETI) {
        *output_reg = value_a;
    } else if(instruction->opcode == OPCODE_GTIR) {
        *output_reg = value_a > register_b ? 1 : 0;
    } else if(instruction->opcode == OPCODE_GTRI) {
        *output_reg = register_a > value_b ? 1 : 0;
    } else if(instruction->opcode == OPCODE_GTRR) {
        *output_reg = register_a > register_b ? 1 : 0;
    } else if(instruction->opcode == OPCODE_EQIR) {
        *output_reg = value_a == register_b ? 1 : 0;
    } else if(instruction->opcode == OPCODE_EQRI) {
        *output_reg = register_a == value_b ? 1 : 0;
    } else if(instruction->opcode == OPCODE_EQRR) {
        *output_reg = register_a == register_b ? 1 : 0;
    }
    
    p->ip = p->registers[p->ip_reg];
    p->ip++;
    
    //print_registers(p->registers);
    //printf("\n");
}

int main() {
    Program p;
    init_program(&p);
    
    read_program(&p, "input.txt");
    
    while(is_ip_valid(&p)) {
        if(p.ip == 12) {
            print_registers(p.registers);
            printf("\n");
        }
        
        execute_instruction(&p);
        
        
    }
    
    printf("Value in register 0: %d\n", p.registers[0]);
    
    return 0;
}
