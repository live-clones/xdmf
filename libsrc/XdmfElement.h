/*******************************************************************/
/*                               XDMF                              */
/*                   eXtensible Data Model and Format              */
/*                                                                 */
/*  Id : Id  */
/*  Date : $Date$ */
/*  Version : $Revision$ */
/*                                                                 */
/*  Author:                                                        */
/*     Jerry A. Clarke                                             */
/*     clarke@arl.army.mil                                         */
/*     US Army Research Laboratory                                 */
/*     Aberdeen Proving Ground, MD                                 */
/*                                                                 */
/*     Copyright @ 2006 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/
#ifndef __XdmfElement_h
#define __XdmfElement_h

#include "XdmfLightData.h"

#define XDMF_ELEMENT_STATE_UNINITIALIZED 0
#define XDMF_ELEMENT_STATE_LIGHT_PARSED  1
#define XDMF_ELEMENT_STATE_HEAVY_READ    2

class XdmfDOM;

/*!
    XdmfElement represents an Element in the LightData. For example,
    XdmfInformation, XdmfGrid, XdmfTopology etc. are all elements.
    Elements have "Attributes" which are the Name=Value pairs in the
    XML. Elements can also have children elements and CDATA which is the
    Character Data of the Element. Consider :
\verbatim

    <Element Name="Stuff">
        November 28, 1962
        <Element Name="XBounds" Value="0.0 100.0" />
    </Element>

\endverbatim
    In the first Element, Name is the one and only Attribute. The second
    Element is a child of the first and has the Attributes : Name and Value.
    November 28, 1962 is the CDATA of the first Element.
*/
class XDMF_EXPORT XdmfElement : public XdmfLightData {

public:
    XdmfElement();
    ~XdmfElement();
    virtual XdmfConstString GetClassName() { return("XdmfElement"); } ;

    //! Set the DOM to use
    XdmfSetValueMacro(DOM, XdmfDOM *);
    //! Get the current DOM
    XdmfGetValueMacro(DOM, XdmfDOM *);

    //! Set the current State
    XdmfSetValueMacro(State, XdmfInt32 );
    //! Get the current State
    XdmfGetValueMacro(State, XdmfInt32 );

    //! Does this Element Reference Some Other Element
    XdmfSetValueMacro(IsReference, XdmfInt32);
    //! Does this Element Reference Some Other Element
    XdmfGetValueMacro(IsReference, XdmfInt32);

    //! Reference another XML node
    /*!
    \param Element is the head XML node of a potential reference chain
    */
    virtual XdmfXmlNode CheckForReference(XdmfXmlNode Element);

    //! Add a child Node
    XdmfInt32 InsertChildElement(XdmfXmlNode Child);

    //! Return the XML representation
    XdmfConstString Serialize();

//! Set the XML Node from which to parse
/*! 
        \param Element is the low level node returned from XdmfDOM->FindElement() etc.
*/
    XdmfInt32   SetElement(XdmfXmlNode Element);
//! Get the XML Node
    XdmfGetValueMacro(Element, XdmfXmlNode);

    //! Get the Element type : Grid, Topology, etc.
    XdmfConstString GetElementType();

    //! Initialize basic structure from XML (INPUT)
    virtual XdmfInt32 UpdateInformation();

    //! Initialize all information. Possibly acessing Heavy Data. (INPUT)
    virtual XdmfInt32 Update();

    //! Update the DOM from the Basic Structure
    virtual XdmfInt32 Build();

    //! Set the Value of an Attribute (OUTPUT)
    XdmfInt32 Set(XdmfConstString Name, XdmfConstString Value);

    //! Get the Value of An Attribute (INPUT)
    XdmfConstString Get(XdmfConstString Name);

    //! Copy Information from Another Element. Overridden in Child Class
    virtual XdmfInt32 Copy(XdmfElement *Source);

    //! Follow a Refernce Chain one step, if it exists
    XdmfXmlNode FollowReference(XdmfXmlNode Element);

protected:
    void        SetReferenceObject(XdmfXmlNode Element, void *p);
    void        *GetReferenceObject(XdmfXmlNode Element);
    XdmfDOM     *DOM;
    XdmfInt32   State;
    //! Target XML That Represents this. In the case of a reference, this is the target XML not the Reference="XX" node.
    XdmfXmlNode Element;
    //! If this is a Reference XML, this is the head of the Reference chain (the Original XML node).
    XdmfXmlNode ReferenceElement;
    XdmfInt32   IsReference;
};
#endif // __XdmfElement_h
