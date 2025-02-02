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


#ifndef _ANCHOREDDRAWOBJECT_HXX
#define _ANCHOREDDRAWOBJECT_HXX

#include <anchoredobject.hxx>
#include <tools/gen.hxx>

/** class for the positioning of drawing objects

    OD 2004-03-25 #i26791#

    @author OD
*/
class SW_DLLPUBLIC SwAnchoredDrawObject : public SwAnchoredObject
{
    private:
        // boolean, indicating that the object position has been invalidated
        // and that a positioning has to be performed.
        bool mbValidPos;

        // rectangle, keeping the last object rectangle after the positioning
        // --> OD 2004-09-29 #i34748# - change <maLastObjRect> to a pointer
        Rectangle* mpLastObjRect;

        // boolean, indicating that anchored drawing object hasn't been attached
        // to a anchor frame yet. Once, it is attached to a anchor frame the
        // boolean changes its state.
        bool mbNotYetAttachedToAnchorFrame;

        // --> OD 2004-08-09 #i28749# - boolean, indicating that anchored
        // drawing object hasn't been positioned yet. Once, it's positioned the
        // boolean changes its state.
        bool mbNotYetPositioned;

        // --> OD 2006-03-17 #i62875#
        // boolean, indicating that after change of layout direction the
        // anchored drawing object has to be captured on the page, if it exceeds
        // the left or right page margin.
        // Needed for compatibility option <DoNotCaptureDrawObjsOnPage>
        bool mbCaptureAfterLayoutDirChange;
        // <--

        /** method for the intrinsic positioning of a at-paragraph|at-character
            anchored drawing object

            OD 2004-08-12 #i32795# - helper method for method <MakeObjPos>

            @author OD
        */
        void _MakeObjPosAnchoredAtPara();

        /** method for the intrinsic positioning of a at-page|at-frame anchored
            drawing object

            OD 2004-08-12 #i32795# - helper method for method <MakeObjPos>

            @author OD
        */
        void _MakeObjPosAnchoredAtLayout();

            /** method to set positioning attributes (not for as-character anchored)

            OD 2004-10-20 #i35798#
            During load the positioning attributes aren't set.
            Thus, the positioning attributes are set by the current object geometry.
            This method is also used for the conversion for drawing objects
            (not anchored as-character) imported from OpenOffice.org file format
            once and directly before the first positioning.

            @author OD
        */
        void _SetPositioningAttr();

        /** method to set internal anchor position of <SdrObject> instance
            of the drawing object

            For drawing objects the internal anchor position of the <SdrObject>
            instance has to be set.
            Note: This adjustment is not be done for as-character anchored
            drawing object - the positioning code takes care of this.
            OD 2004-07-29 #i31698# - API for drawing objects in Writer has
            been adjusted. Thus, this method will only set the internal anchor
            position of the <SdrObject> instance to the anchor position given
            by its anchor frame.

            @author OD
        */
        void _SetDrawObjAnchor();

        /** method to invalidate the given page frame

            OD 2004-07-02 #i28701#

            @author OD
        */
        void _InvalidatePage( SwPageFrm* _pPageFrm );

    protected:
        virtual void ObjectAttachedToAnchorFrame();

        /** method to assure that anchored object is registered at the correct
            page frame

            OD 2004-07-02 #i28701#

            @author OD
        */
        virtual void RegisterAtCorrectPage();

        // --> OD 2006-08-10 #i68520#
        virtual bool _SetObjTop( const SwTwips _nTop);
        virtual bool _SetObjLeft( const SwTwips _nLeft);
        // <--

        // --> OD 2006-10-05 #i70122#
        virtual const SwRect GetObjBoundRect() const;
        // <--
    public:
        TYPEINFO();

        SwAnchoredDrawObject();
        virtual ~SwAnchoredDrawObject();

        // declaration of pure virtual methods of base class <SwAnchoredObject>
        virtual void MakeObjPos();
        virtual void InvalidateObjPos();
        inline bool IsValidPos() const
        {
            return mbValidPos;
        }

        // accessors to the format
        virtual SwFrmFmt& GetFrmFmt();
        virtual const SwFrmFmt& GetFrmFmt() const;

        // accessors to the object area and its position
        virtual const SwRect GetObjRect() const;
        // --> OD 2004-09-29 #i34748# - change return type to a pointer.
        // Return value can be NULL.
        const Rectangle* GetLastObjRect() const;
        // <--
        // --> OD 2004-09-29 #i34748# - change method
        void SetLastObjRect( const Rectangle& _rNewObjRect );
        // <--

        /** adjust positioning and alignment attributes for new anchor frame

            OD 2004-04-21
            Set horizontal and vertical position/alignment to manual position
            relative to anchor frame area using the anchor position of the
            new anchor frame and the current absolute drawing object position.
            Note: For correct Undo/Redo method should only be called inside a
            Undo-/Redo-action.
            OD 2004-08-24 #i33313# - add second optional parameter <_pNewObjRect>

            @author OD

            @param <_pNewAnchorFrm>
            input parameter - new anchor frame for the anchored object.

            @param <_pNewObjRect>
            optional input parameter - proposed new object rectangle. If not
            provided the current object rectangle is taken.
        */
        void AdjustPositioningAttr( const SwFrm* _pNewAnchorFrm,
                                    const SwRect* _pNewObjRect = 0L );

        /** method to notify background of drawing object

            OD 2004-06-30 #i28701#

            @author OD
        */
        virtual void NotifyBackground( SwPageFrm* _pPageFrm,
                                       const SwRect& _rRect,
                                       PrepareHint _eHint );

        // --> OD 2005-08-16 #i53320#
        inline bool NotYetPositioned() const
        {
            return mbNotYetPositioned;
        }
        // <--

        // --> OD 2006-03-17 #i62875#
        // change of layout direction needs to be tracked
        // for setting <mbCaptureAfterLayoutDirChange>.
        virtual void UpdateLayoutDir();
        // <--
        // --> OD 2006-03-17 #i62875#
        bool IsOutsidePage() const;
        // <--

        // new Loop control
        void ValidateThis() { mbValidPos = true; }
};

#endif
