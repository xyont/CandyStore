#include "elset.h"

elSet::elSet()
{
}

long elSet::getIndex(long searchElement, int* exists)
{
unsigned long links=1;
unsigned long rechts=elLabels.size();
unsigned long mitte;

        while(1)
        {
        if (links>rechts)
            {
            *exists=0;
            return(rechts);
            }
        mitte=(links+rechts)/2;
        if (elLabels[mitte-1]==searchElement)
        {
            *exists=1;
            return(mitte-1);
        }

        if (searchElement<elLabels[mitte-1])
                rechts=mitte-1;
        else
                links=mitte+1;
        }
}
