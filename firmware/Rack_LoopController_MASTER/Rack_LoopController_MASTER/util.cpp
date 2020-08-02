/*
 * util.cpp
 *
 * Created: 26-01-2020 19:36:30
 *  Author: Esben
 */ 

#include "util.h"

uint8_t Util_ReverseByte(uint8_t b)
{
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}

uint8_t Util_Bit2Int(uint8_t val)
{
	uint8_t i = 0;
	
	while (val)
	{
		val = (val >> 1);
		i++;
	}
	
	return i;
}

/* Returns array index whose value is closest to target
** (Array must be sorted in decending order) */
uint8_t FindClosestArrayIndexDecending(const uint32_t *arr, uint8_t n, uint32_t target)
{
    // Corner cases
    if (target >= arr[0])
        return 0;
    if (target <= arr[n - 1])
        return (n - 1);
 
    uint8_t i = 0;
    uint8_t j = n;
    uint8_t mid = 0;

    // Do binary search
    while (i < j)
    {
        mid = ((i + j) >> 1);   // (i + j) / 2
 
        if (arr[mid] == target)
            return mid;
 
        /* If target is greater than array element, then search in left */
        if (target > arr[mid])
        {
            // If target is less than previous to mid, return closest of two
            if (mid > 0 && target < arr[mid - 1])
            {
                if (target - arr[mid] >= arr[mid - 1] - target)
                    return (mid - 1);
                else
                    return mid;
            }
 
            /* Repeat for left half */
            j = mid;
        }
        // If target is less than mid
        else
        {
            // If target is greater than next from mid, return closest of two
            if (mid < (n - 1) && target > arr[mid + 1])
            {
                if (target - arr[mid + 1] >= arr[mid] - target)
                    return mid;
                else
                    return (mid + 1);
            }

            // update i
            i = mid + 1; 
        }
    }
 
    // Only single element left after search
    return mid;
}

/* Returns the value that is most numerous in the array */
uint8_t GetMostCommon(uint8_t *arr, uint8_t size)
{
    uint8_t count = 0;
    uint8_t most = 0;
    uint8_t temp = 0;
    uint8_t elem = 0;

    for (uint8_t i = 0; i < size; i++)
    {
        temp = arr[i];
        count = 1;

        for (uint8_t j = i+1; j < size; j++)
        {
            if (arr[j] == temp)
                count++;
        }

        if (most < count)
        {
            most = count;
            elem = arr[i];
        }
    }

    return elem;
}