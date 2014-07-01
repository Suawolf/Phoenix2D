#!/bin/bash

if [ "$1" == "localization" ] 
then
	./agent test localization &
	./agent trainer localization.phx
elif [ "$1" == "dribble" ]
then
	./agent test dribble &
	./agent trainer dribble.phx
fi