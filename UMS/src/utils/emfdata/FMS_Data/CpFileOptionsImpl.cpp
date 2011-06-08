// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * FMS_Data/CpFileOptionsImpl.cpp
 * Copyright (C) Cátedra SAES-UMU 2010 <andres.senac@um.es>
 *
 * EMF4CPP is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EMF4CPP is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "CpFileOptions.hpp"
#include <FMS_Data/FMS_DataPackage.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include <ecore/EStructuralFeature.hpp>
#include <ecore/EReference.hpp>
#include <ecore/EObject.hpp>
#include <ecorecpp/mapping.hpp>

using namespace ::FMS_Data;

/*PROTECTED REGION ID(CpFileOptionsImpl.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

void CpFileOptions::_initialize()
{
    // Supertypes

    // Rerefences

    /*PROTECTED REGION ID(CpFileOptionsImpl__initialize) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

// Operations


// EObject
::ecore::EJavaObject CpFileOptions::eGet(::ecore::EInt _featureID,
        ::ecore::EBoolean _resolve)
{
    ::ecore::EJavaObject _any;
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::CPFILEOPTIONS__ISRECURSIVE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EBoolean >::toAny(_any,
                m_isRecursive);
    }
        return _any;
    case ::FMS_Data::FMS_DataPackage::CPFILEOPTIONS__TRCOMMAND:
    {
        ::ecorecpp::mapping::any_traits< ::FMS_Data::TransferCommand >::toAny(
                _any, m_trCommand);
    }
        return _any;

    }
    throw "Error";
}

void CpFileOptions::eSet(::ecore::EInt _featureID,
        ::ecore::EJavaObject const& _newValue)
{
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::CPFILEOPTIONS__ISRECURSIVE:
    {
        ::ecorecpp::mapping::any_traits< ::ecore::EBoolean >::fromAny(
                _newValue, m_isRecursive);
    }
        return;
    case ::FMS_Data::FMS_DataPackage::CPFILEOPTIONS__TRCOMMAND:
    {
        ::ecorecpp::mapping::any_traits< ::FMS_Data::TransferCommand >::fromAny(
                _newValue, m_trCommand);
    }
        return;

    }
    throw "Error";
}

::ecore::EBoolean CpFileOptions::eIsSet(::ecore::EInt _featureID)
{
    switch (_featureID)
    {
    case ::FMS_Data::FMS_DataPackage::CPFILEOPTIONS__ISRECURSIVE:
        return m_isRecursive != false;
    case ::FMS_Data::FMS_DataPackage::CPFILEOPTIONS__TRCOMMAND:
        return m_trCommand != 2;

    }
    throw "Error";
}

void CpFileOptions::eUnset(::ecore::EInt _featureID)
{
    switch (_featureID)
    {

    }
    throw "Error";
}

::ecore::EClass_ptr CpFileOptions::_eClass()
{
    static ::ecore::EClass_ptr
            _eclass =
                    dynamic_cast< ::FMS_Data::FMS_DataPackage_ptr > (::FMS_Data::FMS_DataPackage::_instance())->getCpFileOptions();
    return _eclass;
}

