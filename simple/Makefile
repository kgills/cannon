CC=gcc
C_FLAGS=-O4 -mcmodel=medium
OUTPUT=cannon.out

all: cannon.c
	$(CC) $(C_FLAGS) $^ -o $(OUTPUT)

clean:
	@rm -f $(OUTPUT)
	@rm -f slurm*
