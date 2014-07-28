#!/bin/bash
if [ "$1" == "prueba1" ]
then
	if [ "$2" == "fuzzy" ]
	then
			./agent test prueba1 Fuzzy & sleep 1
			./agent test prueba1 Dummy & sleep 1
			./agent test prueba1 Dummy & sleep 1
			./agent test prueba1 Dummy & sleep 1
			./agent test prueba1 Dummy & sleep 1
			./agent test prueba1 Dummy & sleep 1
			./agent test prueba1 Dummy & sleep 1
			./agent trainer trainer/prueba1.phx
	elif [ "$2" == "potential" ]
	then
			./agent test prueba1 Potential & sleep 1
			./agent test prueba1 Dummy & sleep 1
			./agent test prueba1 Dummy & sleep 1
			./agent test prueba1 Dummy & sleep 1
			./agent test prueba1 Dummy & sleep 1
			./agent test prueba1 Dummy & sleep 1
			./agent test prueba1 Dummy & sleep 1
			./agent trainer trainer/prueba1.phx
	fi
elif [ "$1" == "prueba2" ] 
then
	if [ "$2" == "fuzzy" ]
	then
			./agent test prueba2 Fuzzy & sleep 1
			./agent test prueba2 Dummy & sleep 1
			./agent test prueba2 Dummy & sleep 1
			./agent test prueba2 Dummy & sleep 1
			./agent trainer trainer/prueba2.phx
	elif [ "$2" == "potential" ]
	then
			./agent test prueba2 Potential & sleep 1
			./agent test prueba2 Dummy & sleep 1
			./agent test prueba2 Dummy & sleep 1
			./agent test prueba2 Dummy & sleep 1
			./agent trainer trainer/prueba2.phx
	fi
elif [ "$1" == "prueba3" ] 
then
	if [ "$2" == "fuzzy" ]
	then
			./agent test prueba3 Fuzzy & sleep 1
			./agent test prueba3 Dummy & sleep 1
			./agent trainer trainer/prueba3.phx
	elif [ "$2" == "potential" ]
	then
			./agent test prueba3 Potential & sleep 1
			./agent test prueba3 Dummy & sleep 1
			./agent trainer trainer/prueba2.phx
	fi
else
	echo "Pruebas MICAI 2014"
	echo "Escriba: prueba # para definir el número de prueba"
	echo "Seguido escriba fuzzy o potential para escoger entre"
	echo "algoritmo difuso o campos potenciales respectivamente"
	echo "Prueba 1: Consumo de Stamina"
	echo "Prueba 2: Evasion de obstaculos"
	echo "Prueba 3: Oponente agresivo"
	echo "usage: ./suatest.sh <PRUEBA> <METODO>"
fi
