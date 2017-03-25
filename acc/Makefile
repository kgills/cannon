CC=pgcc
C_FLAGS=-O4 -mcmodel=medium -acc -Munroll -ta=tesla,8.0
C_FLAGS+=-Minfo=accel
OUTPUT=cannon.out

all: cannon.c
	$(CC) $(C_FLAGS) $^ -o $(OUTPUT)

clean:
	@rm -f $(OUTPUT)
	@rm -f slurm*
