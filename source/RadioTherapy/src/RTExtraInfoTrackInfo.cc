#include "RTExtraInfoTrackInfo.hh"

// --------------------------------------------------------------
RTExtraInfoTrackInfo::RTExtraInfoTrackInfo( iaea_record_type *p_iaea_read )
{
  if( p_iaea_read->iextralong > 0 ) {
    theExtraInfoLong1 = p_iaea_read->extralong[0];
  }
  if( p_iaea_read->iextralong > 1 ) {
    theExtraInfoLong2 = p_iaea_read->extralong[1];
  }
  if( p_iaea_read->iextrafloat > 0 ) {
    theExtraInfoFloat1 = p_iaea_read->extrafloat[0];
  }
  if( p_iaea_read->iextrafloat > 1 ) {
    theExtraInfoFloat2 = p_iaea_read->extrafloat[1];
  }
}


// --------------------------------------------------------------
RTExtraInfoTrackInfo::~RTExtraInfoTrackInfo() 
{
} 
