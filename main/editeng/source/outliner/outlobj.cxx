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
#include "precompiled_editeng.hxx"

#include <outl_pch.hxx>

#define _OUTLINER_CXX
#include <editeng/outliner.hxx>
#include <editeng/outlobj.hxx>
#include <outleeng.hxx>
#include <editeng/editobj.hxx>
#include <vcl/bitmap.hxx>
#include <tools/stream.hxx>

//////////////////////////////////////////////////////////////////////////////

class ImplOutlinerParaObject
{
public:
    // data members
    EditTextObject*                 mpEditTextObject;
    ParagraphDataVector             maParagraphDataVector;
    bool                            mbIsEditDoc;

    // refcounter
	sal_uInt32						mnRefCount;

    // constuctor
    ImplOutlinerParaObject(EditTextObject* pEditTextObject, const ParagraphDataVector& rParagraphDataVector, bool bIsEditDoc)
    :   mpEditTextObject(pEditTextObject),
        maParagraphDataVector(rParagraphDataVector),
        mbIsEditDoc(bIsEditDoc),
        mnRefCount(0)
    {
        if( (maParagraphDataVector.size() == 0) && (pEditTextObject->GetParagraphCount() != 0) )
            maParagraphDataVector.resize(pEditTextObject->GetParagraphCount());
    }

    // destructor
    ~ImplOutlinerParaObject()
    {
        delete mpEditTextObject;
    }

    bool operator==(const ImplOutlinerParaObject& rCandidate) const
	{
		return (*mpEditTextObject == *rCandidate.mpEditTextObject
            && maParagraphDataVector == rCandidate.maParagraphDataVector
            && mbIsEditDoc == rCandidate.mbIsEditDoc);
	}

    // #i102062#
    bool isWrongListEqual(const ImplOutlinerParaObject& rCompare) const
    {
		return mpEditTextObject->isWrongListEqual(*rCompare.mpEditTextObject);
    }
};

//////////////////////////////////////////////////////////////////////////////

void OutlinerParaObject::ImplMakeUnique()
{
    if(mpImplOutlinerParaObject->mnRefCount)
    {
        ImplOutlinerParaObject* pNew = new ImplOutlinerParaObject(
            mpImplOutlinerParaObject->mpEditTextObject->Clone(),
            mpImplOutlinerParaObject->maParagraphDataVector,
            mpImplOutlinerParaObject->mbIsEditDoc);
        mpImplOutlinerParaObject->mnRefCount--;
        mpImplOutlinerParaObject = pNew;
    }
}

OutlinerParaObject::OutlinerParaObject(const EditTextObject& rEditTextObject, const ParagraphDataVector& rParagraphDataVector, bool bIsEditDoc)
:   mpImplOutlinerParaObject(new ImplOutlinerParaObject(rEditTextObject.Clone(), rParagraphDataVector, bIsEditDoc))
{
}

OutlinerParaObject::OutlinerParaObject( const EditTextObject& rEditTextObject)
:   mpImplOutlinerParaObject( new ImplOutlinerParaObject( rEditTextObject.Clone(), ParagraphDataVector(), true))
{}

OutlinerParaObject::OutlinerParaObject(const OutlinerParaObject& rCandidate)
:   mpImplOutlinerParaObject(rCandidate.mpImplOutlinerParaObject)
{
	mpImplOutlinerParaObject->mnRefCount++;
}

OutlinerParaObject::~OutlinerParaObject()
{
	if(mpImplOutlinerParaObject->mnRefCount)
	{
		mpImplOutlinerParaObject->mnRefCount--;
	}
	else
	{
		delete mpImplOutlinerParaObject;
	}
}

OutlinerParaObject& OutlinerParaObject::operator=(const OutlinerParaObject& rCandidate)
{
	if(rCandidate.mpImplOutlinerParaObject != mpImplOutlinerParaObject)
	{
		if(mpImplOutlinerParaObject->mnRefCount)
		{
			mpImplOutlinerParaObject->mnRefCount--;
		}
		else
		{
			delete mpImplOutlinerParaObject;
		}
		
		mpImplOutlinerParaObject = rCandidate.mpImplOutlinerParaObject;
		mpImplOutlinerParaObject->mnRefCount++;
	}

	return *this;
}

bool OutlinerParaObject::operator==(const OutlinerParaObject& rCandidate) const
{
	if(rCandidate.mpImplOutlinerParaObject == mpImplOutlinerParaObject)
	{
		return true;
	}

    return (*rCandidate.mpImplOutlinerParaObject == *mpImplOutlinerParaObject);
}

// #i102062#
bool OutlinerParaObject::isWrongListEqual(const OutlinerParaObject& rCompare) const
{
	if(rCompare.mpImplOutlinerParaObject == mpImplOutlinerParaObject)
	{
		return true;
	}

    return mpImplOutlinerParaObject->isWrongListEqual(*rCompare.mpImplOutlinerParaObject);
}

sal_uInt16 OutlinerParaObject::GetOutlinerMode() const
{
	return mpImplOutlinerParaObject->mpEditTextObject->GetUserType();
}

void OutlinerParaObject::SetOutlinerMode(sal_uInt16 nNew)
{
    if(mpImplOutlinerParaObject->mpEditTextObject->GetUserType() != nNew)
    {
        ImplMakeUnique();
    	mpImplOutlinerParaObject->mpEditTextObject->SetUserType(nNew);
    }
}

bool OutlinerParaObject::IsVertical() const
{
	return mpImplOutlinerParaObject->mpEditTextObject->IsVertical();
}

void OutlinerParaObject::SetVertical(bool bNew)
{
	if((bool)mpImplOutlinerParaObject->mpEditTextObject->IsVertical() != bNew)
    {
        ImplMakeUnique();
    	mpImplOutlinerParaObject->mpEditTextObject->SetVertical(bNew);
    }
}

sal_uInt32 OutlinerParaObject::Count() const
{
    return mpImplOutlinerParaObject->maParagraphDataVector.size();
}

sal_Int16 OutlinerParaObject::GetDepth(sal_uInt32 nPara) const
{
    if(nPara < mpImplOutlinerParaObject->maParagraphDataVector.size())
    {
        return mpImplOutlinerParaObject->maParagraphDataVector[nPara].getDepth();
    }
    else
    {
        return -1;
    }
}

const EditTextObject& OutlinerParaObject::GetTextObject() const
{
    return *mpImplOutlinerParaObject->mpEditTextObject;
}

bool OutlinerParaObject::IsEditDoc() const
{
    return mpImplOutlinerParaObject->mbIsEditDoc;
}

const ParagraphData& OutlinerParaObject::GetParagraphData(sal_uInt32 nIndex) const
{
    if(nIndex < mpImplOutlinerParaObject->maParagraphDataVector.size())
    {
        return mpImplOutlinerParaObject->maParagraphDataVector[nIndex];
    }
    else
    {
        OSL_ENSURE(false, "OutlinerParaObject::GetParagraphData: Access out of range (!)");
        static ParagraphData aEmptyParagraphData;
        return aEmptyParagraphData;
    }
}

void OutlinerParaObject::ClearPortionInfo()
{
    ImplMakeUnique();
	mpImplOutlinerParaObject->mpEditTextObject->ClearPortionInfo();
}

bool OutlinerParaObject::ChangeStyleSheets(const XubString& rOldName, SfxStyleFamily eOldFamily, const XubString& rNewName, SfxStyleFamily eNewFamily)
{
    ImplMakeUnique();
	return mpImplOutlinerParaObject->mpEditTextObject->ChangeStyleSheets(rOldName, eOldFamily, rNewName, eNewFamily);
}

void OutlinerParaObject::ChangeStyleSheetName(SfxStyleFamily eFamily, const XubString& rOldName, const XubString& rNewName)
{
    ImplMakeUnique();
	mpImplOutlinerParaObject->mpEditTextObject->ChangeStyleSheetName(eFamily, rOldName, rNewName);
}

void OutlinerParaObject::SetStyleSheets(sal_uInt16 nLevel, const XubString rNewName, const SfxStyleFamily& rNewFamily)
{
    const sal_uInt32 nCount(mpImplOutlinerParaObject->maParagraphDataVector.size());

    if(nCount)
    {
        ImplMakeUnique();
        sal_uInt16 nDecrementer(sal::static_int_cast< sal_uInt16 >(nCount));

        for(;nDecrementer;)
        {
            if(GetDepth(--nDecrementer) == nLevel)
            {
                mpImplOutlinerParaObject->mpEditTextObject->SetStyleSheet(nDecrementer, rNewName, rNewFamily);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
// eof
