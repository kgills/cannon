library(gridExtra)

omp_all = read.csv("../data/cannon_omp_1_1024_all.txt")
acc_all = read.csv("../data/cannon_acc_1_16384.txt")
mpi_all = read.csv("../data/cannon_omp_mpi_16_16384.txt")

# Convert FLOPS to GFLOPS
omp_all$flops = omp_all$flops /1000000000
acc_all$flops = acc_all$flops /1000000000
mpi_all$flops = mpi_all$flops /1000000000

# Delete cores from oacc
acc_all = within(acc_all, rm(cores))
mpi_all = within(mpi_all, rm(cores))

# Aggregate the cores and mean flops
omp_all_ag = aggregate(flops~cores, omp_all, mean)

# Add the speedup
omp_all_ag$speedup = omp_all_ag$flops/omp_all_ag$flops[1]

# Add the serial portion
omp_all_ag$serial = (omp_all_ag$speedup - omp_all_ag$cores)/(1 - omp_all_ag$cores)

# Add the efficiency 
omp_all_ag$karp = (((1/omp_all_ag$speedup)-(1/omp_all_ag$cores))/(1 - (1/omp_all_ag$cores)))

# Create plots for the omp_all flops
png("omp_all.png", width=600, height=500)
plot(flops~cores, omp_all_ag, main="Cores vs FLOPS ", xlab="Cores", 
	ylab="Giga FLOPS", col="red", pch=16)
dev.off()

# Create plot for the karp-flatt metic
png("karp_flatt_graph.png")
plot(karp~cores, omp_all_ag, col="blue", pch=16, main="Karp-Flatt metric", xlab="Cores")
dev.off()

png("acc.png", width=400, height=400) 
grid.table(acc_all, rows=NULL)
dev.off()

png("mpi.png", width=400, height=400) 
grid.table(mpi_all, rows=NULL)
dev.off()
