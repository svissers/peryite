export LD_LIBRARY_PATH=~/boost_1_66_0/stage/lib/
cd ../../cmake-build-release/installed
./bin/stride -c $1 -p rng_seed=$2 -p rng_type=$3
