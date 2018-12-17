/*
 * pt100.c
 *
 *  Created on: Dec 1, 2018
 *      Author: matias
 */

#include "pt100.h"

uint16_t search_pt100_list(uint16_t ohmsX100)
{
    uint16_t lower = 0 ;
    uint16_t upper = PT100_TABLE_MAXIDX;
    uint16_t mid = (lower + upper) / 2 ;
    uint16_t pt100val = 0;

    do
    {
        pt100val = Pt100_table[mid];

        if (pt100val == ohmsX100)
            break;
        else if (pt100val < ohmsX100)
            lower = mid + 1 ;
        else
            upper = mid ;
        mid = (lower + upper) / 2 ;

    } while (lower < upper) ;

    return(mid);
}

uint16_t ohmsX100_to_celsius (uint16_t ohmsX100)
{
    uint16_t R_upper = 0;
    uint16_t R_lower = 0;
    uint16_t hundredths = 0;
    uint16_t iTemp = 0;
    uint16_t celsius = 0;
    uint16_t Rnext = 0;
    uint16_t index = search_pt100_list(ohmsX100) ;

    iTemp = index - 1 + CELSIUS_MIN ;

    R_lower = Pt100_table[index - 1];
    R_upper = Pt100_table[index];

    if (ohmsX100 == R_upper)
    {
        iTemp++;
        hundredths = 0;
    }
    else if (ohmsX100 < R_upper)
        hundredths = ((UN_DECIMAL * (ohmsX100 - R_lower)) / (R_upper - R_lower));
    else if (ohmsX100 > R_upper)
    {
        iTemp++ ;
        Rnext = Pt100_table[index + 1];
        hundredths = (UN_DECIMAL * (ohmsX100 - R_upper)) / (Rnext - R_upper);
    }
    else
        hundredths = ((UN_DECIMAL * (ohmsX100 - R_lower)) / (R_upper - R_lower));

    celsius  = iTemp*UN_DECIMAL + hundredths;

    return(celsius);
}

int16_t celsius (uint16_t ohmsX100)
{
    if (ohmsX100 <= Pt100_table[0])
        return(CELSIUS_MIN*UN_DECIMAL);
    else if (ohmsX100 >= Pt100_table[PT100_TABLE_MAXIDX])
        return(CELSIUS_MAX*UN_DECIMAL);
    else
        return(ohmsX100_to_celsius(ohmsX100)) ;
}
