#ifndef _rand
#define _rand

#include "piano_1_0.h"
#include "piano_1_1.h"
#include "piano_1_2.h"
#include "piano_1_3.h"
#include "piano_1_4.h"
#include "piano_1_5.h"
#include "piano_1_6.h"
#include "piano_1_7.h"
#include "piano_1_8.h"
#include "piano_1_9.h"
#include "piano_1_10.h"
#include "piano_1_11.h"
#include "piano_1_12.h"
#include "piano_1_13.h"
#include "piano_1_14.h"
#include "piano_1_15.h"
#include "piano_1_16.h"
#include "piano_1_17.h"
#include "piano_1_18.h"
#include "piano_1_19.h"
#include "piano_1_20.h"
#include "piano_1_21.h"
#include "piano_1_22.h"
#include "piano_1_23.h"
#include "piano_1_24.h"
#include "piano_1_25.h"
#include "piano_1_26.h"
#include "piano_1_27.h"
#include "piano_1_28.h"
#include "piano_1_29.h"
#include "piano_1_30.h"
#include "piano_1_31.h"

const unsigned int Rand_TablePtrsCount = 32;

static HiResWTSet* Rand_TablePtrs[Rand_TablePtrsCount] = { &piano_1_0_TableSet, &piano_1_1_TableSet, &piano_1_2_TableSet, &piano_1_3_TableSet, &piano_1_4_TableSet, &piano_1_5_TableSet, &piano_1_6_TableSet, &piano_1_7_TableSet, &piano_1_8_TableSet, &piano_1_9_TableSet, &piano_1_10_TableSet, &piano_1_11_TableSet, &piano_1_12_TableSet, &piano_1_13_TableSet, &piano_1_14_TableSet, &piano_1_15_TableSet, &piano_1_16_TableSet, &piano_1_17_TableSet, &piano_1_18_TableSet, &piano_1_19_TableSet, &piano_1_20_TableSet, &piano_1_21_TableSet, &piano_1_22_TableSet, &piano_1_23_TableSet, &piano_1_24_TableSet, &piano_1_25_TableSet, &piano_1_26_TableSet, &piano_1_27_TableSet, &piano_1_28_TableSet, &piano_1_29_TableSet, &piano_1_30_TableSet, &piano_1_31_TableSet }; 

// --- Define Wavform Names: Here is where you can override the table names all at once;
//     Remember to keep the names short and simple, refrain from the underscore, and keep the character count below 32 for compatibiltiy with brick files.
static std::string Rand_TableNames[Rand_TablePtrsCount] = { 
	"piano 1 0",
	"piano 1 1",
	"piano 1 2",
	"piano 1 3",
	"piano 1 4",
	"piano 1 5",
	"piano 1 6",
	"piano 1 7",
	"piano 1 8",
	"piano 1 9",
	"piano 1 10",
	"piano 1 11",
	"piano 1 12",
	"piano 1 13",
	"piano 1 14",
	"piano 1 15",
	"piano 1 16",
	"piano 1 17",
	"piano 1 18",
	"piano 1 19",
	"piano 1 20",
	"piano 1 21",
	"piano 1 22",
	"piano 1 23",
	"piano 1 24",
	"piano 1 25",
	"piano 1 26",
	"piano 1 27",
	"piano 1 28",
	"piano 1 29",
	"piano 1 30",
	"piano 1 31" }; 

// --- This is the bank descriptor; you can initialize the bank with it. 
static BankDescriptor Rand_BankDescriptor(Rand_TablePtrsCount, &Rand_TablePtrs[0], &Rand_TableNames[0]);

#endif // definer
