#!/bin/bash 
# 
#SBATCH  --job-name  OneBlade_PID_A
#SBATCH  --nodes=1
#SBATCH  --ntasks=1
#SBATCH  --ntasks-per-node=1
#SBATCH  --partition prandtl 
#SBATCH  --time=0-4:00:00 


module purge
module load gnu8
module load openmpi3
source /opt/software/openfoam/openfoamv2006/OpenFOAM-v2006/etc/bashrc
. $WM_PROJECT_DIR/bin/tools/RunFunctions

pimpleFoam > log


