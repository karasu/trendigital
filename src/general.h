/**************************************************************************
    This file is part of Tren Digital.

    Tren Digital is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tren Digital is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Tren Digital.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/
#ifndef GENERAL_H
#define GENERAL_H

#define ASSERT(x) Q_ASSERT(x)

#define INVALID -1

#define NUM_FUNCTIONS 16
#define NUM_ADITIONAL_FUNCTIONS 8

// document
#define MAGIC_STRING QString("Fitxer de dades del Tren Digital")
#define MAGIC_NUMBER (qint32)0x00110011
#define CURRENT_FILE_VERSION (qint32)0x01

// loko
#define LIGHT_DOCK 1

#define NUM_FUNCTIONS 16
#define NUM_EXTRA_FUNCTIONS 8

// editlokos
#define LOKO_MAX_ADDRESS 65535

// element
#define MAX_BITMAPS 2
#define POSSIBLE_ADDRESSES_BY_ELEMENT 4
#define DECODER_UNKNOWN -1
#define DECODER_DIGITAL	1
#define DECODER_K83		2
#define DECODER_K84		3
#define DECODER_74460	4

#define TYPE_UNKNOWN		-1
#define TYPE_COMMON_TRACK	1
#define TYPE_DETOUR			2
#define TYPE_SIMPLE_SLIP	3
#define TYPE_SIMPLE_SLIP_TWO_DECODERS	4
#define TYPE_DOUBLE_SLIP	5
#define TYPE_DOUBLE_SLIP_TWO_DECODERS	6
#define TYPE_TRIPLE_SWITCH	7

#define TYPE_SIGNAL			10
#define TYPE_SIGNAL_ONE		11
#define TYPE_SIGNAL_TWO		12
#define TYPE_SIGNAL_THREE	13
#define TYPE_SIGNAL_FOUR	14
#define TYPE_END_TRACK		15
#define TYPE_CROSS			16
#define TYPE_DETACH			17
#define TYPE_TRANSFER_TABLE	18
#define TYPE_TURN_TABLE		19

#define TYPE_FBMODULE		20

// grid
#define DEFAULT_GRID_WIDTH  60
#define DEFAULT_GRID_HEIGHT 40
#define THUMBNAIL_WIDTH 128
#define THUMBNAIL_HEIGHT 128

// route
#define DEFAULT_CT_COUNTER_SECONDS 4

// cell and view
#define CELL_WIDTH 20
#define CELL_HEIGHT 20


#endif // GENERAL_H
