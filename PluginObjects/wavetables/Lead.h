#ifndef _lead
#define _lead

#include "lead_1_0.h"
#include "lead_1_1.h"
#include "lead_1_2.h"
#include "lead_1_3.h"
#include "lead_1_4.h"
#include "lead_1_5.h"
#include "lead_1_6.h"
#include "lead_1_7.h"
#include "lead_1_8.h"
#include "lead_1_9.h"
#include "lead_1_10.h"
#include "lead_1_11.h"
#include "lead_1_12.h"
#include "lead_1_13.h"
#include "lead_1_14.h"
#include "lead_1_15.h"
#include "lead_1_16.h"
#include "lead_1_17.h"
#include "lead_1_18.h"
#include "lead_1_19.h"
#include "lead_1_20.h"
#include "lead_1_21.h"
#include "lead_1_22.h"
#include "lead_1_23.h"
#include "lead_1_24.h"
#include "lead_1_25.h"
#include "lead_1_26.h"
#include "lead_1_27.h"
#include "lead_1_28.h"
#include "lead_1_29.h"
#include "lead_1_30.h"
#include "lead_1_31.h"

const unsigned int Lead_TablePtrsCount = 32;

static HiResWTSet* Lead_TablePtrs[Lead_TablePtrsCount] = { &lead_1_0_TableSet, &lead_1_1_TableSet, &lead_1_2_TableSet, &lead_1_3_TableSet, &lead_1_4_TableSet, &lead_1_5_TableSet, &lead_1_6_TableSet, &lead_1_7_TableSet, &lead_1_8_TableSet, &lead_1_9_TableSet, &lead_1_10_TableSet, &lead_1_11_TableSet, &lead_1_12_TableSet, &lead_1_13_TableSet, &lead_1_14_TableSet, &lead_1_15_TableSet, &lead_1_16_TableSet, &lead_1_17_TableSet, &lead_1_18_TableSet, &lead_1_19_TableSet, &lead_1_20_TableSet, &lead_1_21_TableSet, &lead_1_22_TableSet, &lead_1_23_TableSet, &lead_1_24_TableSet, &lead_1_25_TableSet, &lead_1_26_TableSet, &lead_1_27_TableSet, &lead_1_28_TableSet, &lead_1_29_TableSet, &lead_1_30_TableSet, &lead_1_31_TableSet }; 

// --- Define Wavform Names: Here is where you can override the table names all at once;
//     Remember to keep the names short and simple, refrain from the underscore, and keep the character count below 32 for compatibiltiy with brick files.
static std::string Lead_TableNames[Lead_TablePtrsCount] = { 
	"lead 1 0",
	"lead 1 1",
	"lead 1 2",
	"lead 1 3",
	"lead 1 4",
	"lead 1 5",
	"lead 1 6",
	"lead 1 7",
	"lead 1 8",
	"lead 1 9",
	"lead 1 10",
	"lead 1 11",
	"lead 1 12",
	"lead 1 13",
	"lead 1 14",
	"lead 1 15",
	"lead 1 16",
	"lead 1 17",
	"lead 1 18",
	"lead 1 19",
	"lead 1 20",
	"lead 1 21",
	"lead 1 22",
	"lead 1 23",
	"lead 1 24",
	"lead 1 25",
	"lead 1 26",
	"lead 1 27",
	"lead 1 28",
	"lead 1 29",
	"lead 1 30",
	"lead 1 31" }; 

// --- This is the bank descriptor; you can initialize the bank with it. 
static BankDescriptor Lead_BankDescriptor(Lead_TablePtrsCount, &Lead_TablePtrs[0], &Lead_TableNames[0]);

#endif // definer
