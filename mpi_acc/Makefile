CC=mpicc
C_FLAGS=-O4 -mcmodel=medium -acc -Munroll -ta=tesla,8.0
OUTPUT=cannon.out

all: cannon.c
	$(CC) $(C_FLAGS) $^ -o $(OUTPUT)

clean:
	@rm -f $(OUTPUT)
	@rm -f slurm*
