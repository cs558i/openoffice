/**************************************************************
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_svx.hxx"

// include ---------------------------------------------------------------
#include <tools/stream.hxx>


#include <svx/pageitem.hxx>
#include <editeng/itemtype.hxx>
#include <svx/unomid.hxx>
#include <com/sun/star/style/PageStyleLayout.hpp>
#include <com/sun/star/style/BreakType.hpp>
#include <svl/itemset.hxx>
#include <svx/svxitems.hrc>
#include <svx/dialmgr.hxx>

using namespace ::rtl;
using namespace ::com::sun::star;

// STATIC DATA -----------------------------------------------------------

TYPEINIT1_FACTORY( SvxPageItem, SfxPoolItem , new SvxPageItem(0));

/*--------------------------------------------------------------------
	Description: Constructor
 --------------------------------------------------------------------*/

SvxPageItem::SvxPageItem( const sal_uInt16 nId ) : SfxPoolItem( nId ),

	eNumType	( SVX_ARABIC ),
	bLandscape	( sal_False ),
	eUse		( SVX_PAGE_ALL )
{
}

/*--------------------------------------------------------------------
	Description: Copy-Constructor
 --------------------------------------------------------------------*/

SvxPageItem::SvxPageItem( const SvxPageItem& rItem )
	: SfxPoolItem( rItem )
{
	eNumType 	= rItem.eNumType;
	bLandscape 	= rItem.bLandscape;
	eUse 		= rItem.eUse;
}

/*--------------------------------------------------------------------
	Description: Clone
 --------------------------------------------------------------------*/

SfxPoolItem* SvxPageItem::Clone( SfxItemPool * ) const
{
	return new SvxPageItem( *this );
}

/*--------------------------------------------------------------------
	Description: Query on equality
 --------------------------------------------------------------------*/

int SvxPageItem::operator==( const SfxPoolItem& rAttr ) const
{
	DBG_ASSERT( SfxPoolItem::operator==(rAttr), "unequal types" );
	const SvxPageItem& rItem = (SvxPageItem&)rAttr;
	return ( eNumType 	== rItem.eNumType 	&&
			 bLandscape == rItem.bLandscape &&
			 eUse 		== rItem.eUse );
}

inline XubString GetUsageText( const sal_uInt16 eU )
{
	switch( eU & 0x000f )
	{
		case SVX_PAGE_LEFT:		return SVX_RESSTR(RID_SVXITEMS_PAGE_USAGE_LEFT);
		case SVX_PAGE_RIGHT:	return SVX_RESSTR(RID_SVXITEMS_PAGE_USAGE_RIGHT);
		case SVX_PAGE_ALL:		return SVX_RESSTR(RID_SVXITEMS_PAGE_USAGE_ALL);
		case SVX_PAGE_MIRROR:	return SVX_RESSTR(RID_SVXITEMS_PAGE_USAGE_MIRROR);
		default: return String();
	}
}

//------------------------------------------------------------------------

SfxItemPresentation SvxPageItem::GetPresentation
(
	SfxItemPresentation	ePres,
	SfxMapUnit			/*eCoreUnit*/,
	SfxMapUnit			/*ePresUnit*/,
	XubString&			rText, const IntlWrapper *
)	const
{
	rText.Erase();

	switch( ePres )
	{
		case SFX_ITEM_PRESENTATION_NONE:
			return SFX_ITEM_PRESENTATION_NONE;
		case SFX_ITEM_PRESENTATION_NAMELESS:
		{
			if ( aDescName.Len() )
			{
				rText = aDescName;
				rText += cpDelim;
			}
			DBG_ASSERT( eNumType <= SVX_NUMBER_NONE, "enum overflow" );
			rText += SVX_RESSTR(RID_SVXITEMS_PAGE_NUM_BEGIN + eNumType);
			rText += cpDelim;
			if ( bLandscape )
				rText += SVX_RESSTR(RID_SVXITEMS_PAGE_LAND_TRUE);
			else
				rText += SVX_RESSTR(RID_SVXITEMS_PAGE_LAND_FALSE);
			rText += cpDelim;
			rText += GetUsageText( eUse );
			return SFX_ITEM_PRESENTATION_NAMELESS;
		}
		case SFX_ITEM_PRESENTATION_COMPLETE:
		{
			rText += SVX_RESSTR(RID_SVXITEMS_PAGE_COMPLETE);
			if ( aDescName.Len() )
			{
				rText += aDescName;
				rText += cpDelim;
			}
			DBG_ASSERT( eNumType <= SVX_NUMBER_NONE, "enum overflow" );
			rText += SVX_RESSTR(RID_SVXITEMS_PAGE_NUM_BEGIN + eNumType);
			rText += cpDelim;
			if ( bLandscape )
				rText += SVX_RESSTR(RID_SVXITEMS_PAGE_LAND_TRUE);
			else
				rText += SVX_RESSTR(RID_SVXITEMS_PAGE_LAND_FALSE);
			rText += cpDelim;
			rText += GetUsageText( eUse );
			return SFX_ITEM_PRESENTATION_COMPLETE;
		}
		default: ; // prevent warning
	}
	return SFX_ITEM_PRESENTATION_NONE;
}

//------------------------------------------------------------------------
sal_Bool SvxPageItem::QueryValue( uno::Any& rVal, sal_uInt8 nMemberId ) const
{
//	sal_Bool bConvert = 0!=(nMemberId&CONVERT_TWIPS);
	nMemberId &= ~CONVERT_TWIPS;
	switch( nMemberId )
	{
		case MID_PAGE_NUMTYPE:
		{
			//!	die Konstanten sind nicht mehr in den IDLs ?!?
			rVal <<= (sal_Int16)( eNumType );
		}
		break;
		case MID_PAGE_ORIENTATION:
			//Landscape= sal_True
			rVal = Bool2Any(bLandscape);
		break;
		case MID_PAGE_LAYOUT :
		{
			style::PageStyleLayout eRet;
			switch( eUse & 0x0f )
			{
				case SVX_PAGE_LEFT:		eRet = style::PageStyleLayout_LEFT;		break;
				case SVX_PAGE_RIGHT:	eRet = style::PageStyleLayout_RIGHT;	break;
				case SVX_PAGE_ALL:		eRet = style::PageStyleLayout_ALL;		break;
				case SVX_PAGE_MIRROR:	eRet = style::PageStyleLayout_MIRRORED;	break;
				default:
					DBG_ERROR("What kind of layout is this?");
					return sal_False;
			}
			rVal <<= eRet;
		}
		break;
	}

	return sal_True;
}
//------------------------------------------------------------------------
sal_Bool SvxPageItem::PutValue( const uno::Any& rVal, sal_uInt8 nMemberId )
{
	switch( nMemberId )
	{
		case MID_PAGE_NUMTYPE:
		{
			sal_Int32 nValue = 0;
			if(!(rVal >>= nValue))
				return sal_False;

			eNumType = (SvxNumType)nValue;
		}
		break;
		case MID_PAGE_ORIENTATION:
			bLandscape = Any2Bool(rVal);
		break;
		case MID_PAGE_LAYOUT :
		{
			style::PageStyleLayout eLayout;
			if(!(rVal >>= eLayout))
			{
				sal_Int32 nValue = 0;
				if(!(rVal >>= nValue))
					return sal_False;
				eLayout = (style::PageStyleLayout)nValue;
			}
			eUse &= 0xfff0;
			switch( eLayout )
			{
				case style::PageStyleLayout_LEFT:		eUse |= SVX_PAGE_LEFT ;		break;
				case style::PageStyleLayout_RIGHT:		eUse |= SVX_PAGE_RIGHT ;	break;
				case style::PageStyleLayout_ALL:		eUse |= SVX_PAGE_ALL ;		break;
				case style::PageStyleLayout_MIRRORED:	eUse |= SVX_PAGE_MIRROR ;	break;
				default: ; // prevent warning
			}
		}
		break;
	}
	return sal_True;
}

//------------------------------------------------------------------------

SfxPoolItem* SvxPageItem::Create( SvStream& rStream, sal_uInt16 ) const
{
	XubString sStr;
	sal_uInt8 eType;
	sal_Bool bLand;
	sal_uInt16 nUse;

	// UNICODE: rStream >> sStr;
	rStream.ReadByteString( sStr );

	rStream >> eType;
	rStream >> bLand;
	rStream >> nUse;

	SvxPageItem* pPage = new SvxPageItem( Which() );
	pPage->SetDescName( sStr );
	pPage->SetNumType( (SvxNumType)eType );
	pPage->SetLandscape( bLand );
	pPage->SetPageUsage( nUse );
	return pPage;
}

//------------------------------------------------------------------------

SvStream& SvxPageItem::Store( SvStream &rStrm, sal_uInt16 /*nItemVersion*/ ) const
{
	// UNICODE: rStrm << aDescName;
	rStrm.WriteByteString(aDescName);

	rStrm << (sal_uInt8)eNumType << bLandscape << eUse;
	return rStrm;
}

/*--------------------------------------------------------------------
	Description: HeaderFooterSet
 --------------------------------------------------------------------*/

SvxSetItem::SvxSetItem( const sal_uInt16 nId, const SfxItemSet& rSet ) :

	SfxSetItem( nId, rSet )
{
}

SvxSetItem::SvxSetItem( const SvxSetItem& rItem ) :

	SfxSetItem( rItem )
{
}

SvxSetItem::SvxSetItem( const sal_uInt16 nId, SfxItemSet* _pSet ) :

	SfxSetItem( nId, _pSet )
{
}

SfxPoolItem* SvxSetItem::Clone( SfxItemPool * ) const
{
	return new SvxSetItem(*this);
}

//------------------------------------------------------------------------

SfxItemPresentation SvxSetItem::GetPresentation
(
	SfxItemPresentation	/*ePres*/,
	SfxMapUnit			/*eCoreUnit*/,
	SfxMapUnit			/*ePresUnit*/,
	XubString&			rText, const IntlWrapper *
)	const
{
	rText.Erase();
	return SFX_ITEM_PRESENTATION_NONE;
}

SfxPoolItem* SvxSetItem::Create(SvStream &rStrm, sal_uInt16 /*nVersion*/) const
{
	SfxItemSet* _pSet = new SfxItemSet( *GetItemSet().GetPool(),
										 GetItemSet().GetRanges() );

	_pSet->Load( rStrm );

	return new SvxSetItem( Which(), *_pSet );
}

SvStream& SvxSetItem::Store(SvStream &rStrm, sal_uInt16 nItemVersion) const
{
	GetItemSet().Store( rStrm, nItemVersion );

	return rStrm;
}

/* vim: set noet sw=4 ts=4: */
