#!/bin/sh
# Compile les deux éditions : emeraude-couronnement-vX.Y.gba (classique) et emeraude-couronnement-vX.Y-carriere.gba.
# Usage : tools/couronnement/editions.sh 0.46   (changer d'édition recompile tout : touch de include/config/general.h)
set -e
V=${1:?version}
J=$(getconf _NPROCESSORS_ONLN 2>/dev/null || sysctl -n hw.ncpu)
touch include/config/general.h
make -j"$J"
cp pokeemerald.gba "emeraude-couronnement-v$V.gba"
touch include/config/general.h
make -j"$J" EDITION=carriere
cp pokeemerald.gba "emeraude-couronnement-v$V-carriere.gba"
touch include/config/general.h
