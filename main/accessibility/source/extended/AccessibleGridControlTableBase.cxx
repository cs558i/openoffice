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
#include "precompiled_accessibility.hxx"


#include "accessibility/extended/AccessibleGridControlTableBase.hxx"
#include <svtools/accessibletable.hxx>
#include <tools/multisel.hxx>
#include <comphelper/sequence.hxx>

// ============================================================================

using ::rtl::OUString;

using ::com::sun::star::uno::Reference;
using ::com::sun::star::uno::Sequence;
using ::com::sun::star::uno::Any;

using namespace ::com::sun::star;
using namespace ::com::sun::star::accessibility;
using namespace ::svt;
using namespace ::svt::table;

// ============================================================================

namespace accessibility {

// ============================================================================

DBG_NAME( AccessibleGridControlTableBase )

AccessibleGridControlTableBase::AccessibleGridControlTableBase(
		const Reference< XAccessible >& rxParent,
		IAccessibleTable& rTable,
		AccessibleTableControlObjType eObjType ) :
	GridControlAccessibleElement( rxParent, rTable, eObjType )
{
}

AccessibleGridControlTableBase::~AccessibleGridControlTableBase()
{
}

// XAccessibleContext ---------------------------------------------------------

sal_Int32 SAL_CALL AccessibleGridControlTableBase::getAccessibleChildCount()
	throw ( uno::RuntimeException )
{
	TCSolarGuard aSolarGuard;
	::osl::MutexGuard aGuard( getOslMutex() );
	ensureIsAlive();
	sal_Int32 nChildren = 0;
	if(m_eObjType == TCTYPE_ROWHEADERBAR)
		nChildren = m_aTable.GetRowCount();
	else if(m_eObjType == TCTYPE_TABLE)
		nChildren = m_aTable.GetRowCount()*m_aTable.GetColumnCount();
	else if(m_eObjType == TCTYPE_COLUMNHEADERBAR)
		nChildren = m_aTable.GetColumnCount();
	return nChildren;
}

sal_Int16 SAL_CALL AccessibleGridControlTableBase::getAccessibleRole()
	throw ( uno::RuntimeException )
{
	ensureIsAlive();
	return AccessibleRole::TABLE;
}

// XAccessibleTable -----------------------------------------------------------

sal_Int32 SAL_CALL AccessibleGridControlTableBase::getAccessibleRowCount()
	throw ( uno::RuntimeException )
{
	TCSolarGuard aSolarGuard;
	::osl::MutexGuard aGuard( getOslMutex() );
	ensureIsAlive();
	return m_aTable.GetRowCount();
}

sal_Int32 SAL_CALL AccessibleGridControlTableBase::getAccessibleColumnCount()
	throw ( uno::RuntimeException )
{
	TCSolarGuard aSolarGuard;
	::osl::MutexGuard aGuard( getOslMutex() );
	ensureIsAlive();
	return m_aTable.GetColumnCount();
}

sal_Int32 SAL_CALL AccessibleGridControlTableBase::getAccessibleRowExtentAt(
		sal_Int32 nRow, sal_Int32 nColumn )
	throw ( lang::IndexOutOfBoundsException, uno::RuntimeException )
{
	TCSolarGuard aSolarGuard;
	::osl::MutexGuard aGuard( getOslMutex() );
	ensureIsAlive();
	ensureIsValidAddress( nRow, nColumn );
	return 1; // merged cells not supported
}

sal_Int32 SAL_CALL AccessibleGridControlTableBase::getAccessibleColumnExtentAt(
		sal_Int32 nRow, sal_Int32 nColumn )
	throw ( lang::IndexOutOfBoundsException, uno::RuntimeException )
{
	TCSolarGuard aSolarGuard;
	::osl::MutexGuard aGuard( getOslMutex() );
	ensureIsAlive();
	ensureIsValidAddress( nRow, nColumn );
	return 1; // merged cells not supported
}

Reference< XAccessible > SAL_CALL AccessibleGridControlTableBase::getAccessibleCaption()
	throw ( uno::RuntimeException )
{
	ensureIsAlive();
	return NULL; // not supported
}

Reference< XAccessible > SAL_CALL AccessibleGridControlTableBase::getAccessibleSummary()
	throw ( uno::RuntimeException )
{
	ensureIsAlive();
	return NULL; // not supported
}

sal_Int32 SAL_CALL AccessibleGridControlTableBase::getAccessibleIndex(
		sal_Int32 nRow, sal_Int32 nColumn )
	throw ( lang::IndexOutOfBoundsException, uno::RuntimeException )
{
	TCSolarGuard aSolarGuard;
	::osl::MutexGuard aGuard( getOslMutex() );
	ensureIsAlive();
	ensureIsValidAddress( nRow, nColumn );
	return implGetChildIndex( nRow, nColumn );
}

sal_Int32 SAL_CALL AccessibleGridControlTableBase::getAccessibleRow( sal_Int32 nChildIndex )
	throw ( lang::IndexOutOfBoundsException, uno::RuntimeException )
{
	TCSolarGuard aSolarGuard;
	::osl::MutexGuard aGuard( getOslMutex() );
	ensureIsAlive();
	ensureIsValidIndex( nChildIndex );
	return implGetRow( nChildIndex );
}

sal_Int32 SAL_CALL AccessibleGridControlTableBase::getAccessibleColumn( sal_Int32 nChildIndex )
	throw ( lang::IndexOutOfBoundsException, uno::RuntimeException )
{
	TCSolarGuard aSolarGuard;
	::osl::MutexGuard aGuard( getOslMutex() );
	ensureIsAlive();
	ensureIsValidIndex( nChildIndex );
	return implGetColumn( nChildIndex );
}

// XInterface -----------------------------------------------------------------

Any SAL_CALL AccessibleGridControlTableBase::queryInterface( const uno::Type& rType )
	throw ( uno::RuntimeException )
{
	Any aAny( GridControlAccessibleElement::queryInterface( rType ) );
	return aAny.hasValue() ?
		aAny : AccessibleGridControlTableImplHelper::queryInterface( rType );
}

void SAL_CALL AccessibleGridControlTableBase::acquire() throw ()
{
	GridControlAccessibleElement::acquire();
}

void SAL_CALL AccessibleGridControlTableBase::release() throw ()
{
	GridControlAccessibleElement::release();
}

// XTypeProvider --------------------------------------------------------------

Sequence< uno::Type > SAL_CALL AccessibleGridControlTableBase::getTypes()
	throw ( uno::RuntimeException )
{
	return ::comphelper::concatSequences(
		GridControlAccessibleElement::getTypes(),
		AccessibleGridControlTableImplHelper::getTypes() );
}

Sequence< sal_Int8 > SAL_CALL AccessibleGridControlTableBase::getImplementationId()
	throw ( uno::RuntimeException )
{
	::osl::MutexGuard aGuard( getOslGlobalMutex() );
	static Sequence< sal_Int8 > aId;
	implCreateUuid( aId );
	return aId;
}

// internal helper methods ----------------------------------------------------

sal_Int32 AccessibleGridControlTableBase::implGetChildCount() const
{
	return m_aTable.GetRowCount()*m_aTable.GetColumnCount();
}

sal_Int32 AccessibleGridControlTableBase::implGetRow( sal_Int32 nChildIndex ) const
{
	sal_Int32 nColumns = m_aTable.GetColumnCount();
	return nColumns ? (nChildIndex / nColumns) : 0;
}

sal_Int32 AccessibleGridControlTableBase::implGetColumn( sal_Int32 nChildIndex ) const
{
	sal_Int32 nColumns = m_aTable.GetColumnCount();
	return nColumns ? (nChildIndex % nColumns) : 0;
}

sal_Int32 AccessibleGridControlTableBase::implGetChildIndex(
		sal_Int32 nRow, sal_Int32 nColumn ) const
{
	return nRow * m_aTable.GetColumnCount() + nColumn;
}

void AccessibleGridControlTableBase::implGetSelectedRows( Sequence< sal_Int32 >& rSeq )
{
	sal_Int32 const selectionCount( m_aTable.GetSelectedRowCount() );
	rSeq.realloc( selectionCount );
	for ( sal_Int32 i=0; i<selectionCount; ++i )
		rSeq[i] = m_aTable.GetSelectedRowIndex(i);
}

void AccessibleGridControlTableBase::ensureIsValidRow( sal_Int32 nRow )
	throw ( lang::IndexOutOfBoundsException )
{
	if( nRow >= m_aTable.GetRowCount() )
		throw lang::IndexOutOfBoundsException(
			OUString( RTL_CONSTASCII_USTRINGPARAM( "row index is invalid" ) ), *this );
}

void AccessibleGridControlTableBase::ensureIsValidColumn( sal_Int32 nColumn )
	throw ( lang::IndexOutOfBoundsException )
{
	if( nColumn >= m_aTable.GetColumnCount() )
		throw lang::IndexOutOfBoundsException(
			OUString( RTL_CONSTASCII_USTRINGPARAM("column index is invalid") ), *this );
}

void AccessibleGridControlTableBase::ensureIsValidAddress(
		sal_Int32 nRow, sal_Int32 nColumn )
	throw ( lang::IndexOutOfBoundsException )
{
	ensureIsValidRow( nRow );
	ensureIsValidColumn( nColumn );
}

void AccessibleGridControlTableBase::ensureIsValidIndex( sal_Int32 nChildIndex )
	throw ( lang::IndexOutOfBoundsException )
{
	if( nChildIndex >= implGetChildCount() )
		throw lang::IndexOutOfBoundsException(
			OUString( RTL_CONSTASCII_USTRINGPARAM("child index is invalid") ), *this );
}

// ============================================================================
}	// namespace accessibility
// ============================================================================
