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
/*     Copyright @ 2002 US Army Research Laboratory                */
/*     All Rights Reserved                                         */
/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
/*                                                                 */
/*     This software is distributed WITHOUT ANY WARRANTY; without  */
/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
/*     for more information.                                       */
/*                                                                 */
/*******************************************************************/
#include "XdmfDOM.h"

XdmfDOM::XdmfDOM ( ) {
  
  this->formatter = 0;
  this->encodingName = 0;
  this->unRepFlags = XMLFormatter::UnRep_CharRef;

        // Initialize the XML4C2 system
        XMLPlatformUtils::Initialize();

        this->parser = new DOMParser;

        this->parser->setValidationScheme(DOMParser::Val_Never);
        this->parser->setDoNamespaces(false);
        this->parser->setCreateEntityReferenceNodes(false);
        this->parser->setToCreateXMLDeclTypeNode(true);

}

XdmfDOM::~XdmfDOM ( ) {
  delete this->parser;
        XMLPlatformUtils::Terminate();
}

XdmfInt32
XdmfDOM::IsChild(DOM_Node nodeToCheck) {
  DOM_Document  doc;
  DOM_NodeList  nodeList;
  DOM_Node   node;
  int    i;
  int    retval;

  doc = this->parser->getDocument();
  nodeList = doc.getChildNodes();
  for(i = 0; i < nodeList.getLength(); i++) {
    node = nodeList.item(i);
    if (node == nodeToCheck) { return (1); };
    retval = this->IsChild(nodeToCheck, node);
    if (retval == 1) { return (retval); } 
  }
  return (0);
}

XdmfInt32
XdmfDOM::IsChild(DOM_Node nodeToCheck, DOM_Node startNode) {
  DOM_NodeList  nodeList;
  DOM_Node   node;
  int    i;
  int    retval;

  nodeList = startNode.getChildNodes();
  for(i = 0; i < nodeList.getLength(); i++) {
    node = nodeList.item(i);
    if (node == nodeToCheck) { return (1); };
    retval = this->IsChild(nodeToCheck, node);
    if (retval == 1) { return (retval); } 
  }
  return (0);
}

XdmfInt32
XdmfDOM::GetNumberOfParameters( ) {
  XdmfInt32 numParams;

  numParams = this->GetNumberOfElements("Parameter");
  return (numParams);
}

XdmfInt32
XdmfDOM::GetNumberOfParameters( XdmfString parameterName ) {
  XdmfInt32   numParams;
  DOM_Element  element;
  XdmfInt32  count;
  DOMString  name;
  DOMString  *Name;
  int    i;

  numParams = 0;  
  Name = new DOMString("Name");

  count = this->GetNumberOfElements("Parameter");
  for ( i = 0; i < count; i++ ) {
    element = this->GetElement("Parameter", i);
    name = element.getAttribute(*Name);
    if ( name.equals(parameterName) ) {
      numParams++;
    }
  }
  return (numParams);
}

XdmfInt32
XdmfDOM::GetNumberOfParameters( XdmfString parameterName, DOM_Element elementNode ) {
  XdmfInt32   numParams;
  DOM_Element  element;
  XdmfInt32  count;
  DOMString  name;
  DOMString  *Name;
  int    i;

  numParams = 0;  
  Name = new DOMString("Name");

  count = this->GetNumberOfElements("Parameter", elementNode);
  for ( i = 0; i < count; i++ ) {
    element = this->GetElement("Parameter", i, elementNode);
    name = element.getAttribute(*Name);
    if ( name.equals(parameterName) ) {
      numParams++;
    }
  }
  return (numParams);
}

XdmfInt32
XdmfDOM::GetNumberOfParameters(DOM_Element elementNode) {
  XdmfInt32 numParams;

  numParams = this->GetNumberOfElements("Parameter", elementNode);
  return (numParams);
}

DOM_Element
XdmfDOM::GetParameter ( XdmfString parameterName ) {
  DOM_Element elementNode;  
  DOM_Element element;  
  XdmfInt32 index;  
  
  index = 0;
  elementNode = 0;
  element = this->GetParameter(parameterName, index, elementNode);
  return(element);
}

DOM_Element
XdmfDOM::GetParameter ( XdmfString parameterName, XdmfInt32 index ) {
  DOM_Element elementNode;  
  DOM_Element element;  
  
  elementNode = 0;
  element = this->GetParameter(parameterName, index, elementNode);
  return(element);
}

DOM_Element
XdmfDOM::GetParameter ( XdmfString parameterName, XdmfInt32 index, DOM_Element elementNode) {
  DOM_Element  element;
  DOM_NodeList  nodeList;
  DOM_Document  doc;
  DOMString  *tag;
  DOMString  name;
  DOMString  *Name;
  XdmfInt32  count;
  XdmfInt32  len;
  XdmfInt32  i;

  tag = new DOMString("Parameter");
  Name = new DOMString("Name");
  doc = this->parser->getDocument();

  if (elementNode.isNull()) {
    nodeList = doc.getElementsByTagName(*tag);
  } else {
    nodeList = elementNode.getElementsByTagName(*tag);
  }

  len = nodeList.getLength();
  count = 0;
  
  if (index < len) {
    for(i=0;i<len;i++) {
      element = (DOM_Element&)nodeList.item(i);
      name = element.getAttribute(*Name);
      if ( name.equals(parameterName) ) {
        if (index==count) {
          return(element);
        } else {
          count++;
        }
      }
    }
  }
  element = 0;
  return( element );
}

XdmfInt32
XdmfDOM::GetNumberOfElements( ) {

  DOM_NodeList   nodeList;
  DOM_Document  doc;
  DOMString  *tag;  
  XdmfInt32  length;
  
  tag = new DOMString("*");
  doc = this->parser->getDocument();
  nodeList = doc.getElementsByTagName(*tag);
  length = nodeList.getLength();  
  return( length );

}

XdmfInt32
XdmfDOM::GetNumberOfElements( XdmfString tagName ) {
  DOM_NodeList   nodeList;
  DOMString  *tag;  
  XdmfInt32  length;
  DOM_Document  doc;
  
  tag = new DOMString(tagName);
  doc = this->parser->getDocument();
  nodeList = doc.getElementsByTagName(*tag);
  length = nodeList.getLength();  
  return( length );
}

XdmfInt32
XdmfDOM::GetNumberOfElements( DOM_Element elementNode ) {
  DOM_NodeList  nodeList;
  DOMString  *tag;  
  XdmfInt32  length;
  
  tag = new DOMString("*");
  nodeList = elementNode.getElementsByTagName(*tag);
  length = nodeList.getLength();  
  return( length );
}

XdmfInt32
XdmfDOM::GetNumberOfElements( XdmfString tagName, DOM_Element elementNode ) {
  DOM_NodeList  nodeList;
  DOMString  *tag;  
  XdmfInt32  length;
  
  tag = new DOMString(tagName);
  nodeList = elementNode.getElementsByTagName(*tag);
  length = nodeList.getLength();  
  return( length );
}

DOM_Element
XdmfDOM::GetElement ( XdmfString tagName ) {
  DOM_Element elementNode;  
  DOM_Element element;  
  XdmfInt32 index;  
  
  index = 0;
  elementNode = 0;
  element = this->GetElement(tagName, index, elementNode);
  return(element);
}

DOM_Element
XdmfDOM::GetElement ( XdmfString tagName, XdmfInt32 index ) {
  DOM_Element elementNode;  
  DOM_Element element;  
  
  elementNode = 0;
  element = this->GetElement(tagName, index, elementNode);
  return(element);
}

DOM_Element
XdmfDOM::GetElement ( XdmfString tagName, XdmfInt32 index, DOM_Element elementNode) {
  DOM_Element  element;
  DOM_NodeList  nodeList;
  DOM_Document  doc;
  DOMString  *tag;
  XdmfInt32  len;

  tag = new DOMString(tagName);
  doc = this->parser->getDocument();

  if (elementNode.isNull()) {
    nodeList = doc.getElementsByTagName(*tag);
  } else {
    nodeList = elementNode.getElementsByTagName(*tag);
  }

  len = nodeList.getLength();

  if (index >= len) {
    element = 0;
  } else {
    element = (DOM_Element&)nodeList.item(index);
  }

  return( element );
}

DOM_Element
XdmfDOM::GetProcessingInstructionData( DOM_ProcessingInstruction pi ) {
//
//  This routine assumes that the processing instruction data is 
//  in the form : name = "value" , just like attributes of an 
//  XML element.  This routine places creates a DOM_Element
//  and sets the Attributes of this element to be the data 
//  from the processing instruction.  It returns the DOM_Element.
//
  DOM_Document  doc;
  DOM_Element  element;
  DOM_NodeList  nodeList;
  DOMString  elementTagName;
  DOMString  piData;
  DOMString  name;
  DOMString  value;
  XdmfInt32  length;
  XdmfInt32  i;
  XdmfInt32  start;
  XdmfInt32  end;
  XdmfInt32  firstQuoteFlag;
  XdmfInt32  nameValuePairFlag;
  XMLCh    c;

  doc = this->parser->getDocument();
  elementTagName = pi.getTarget();
  elementTagName.appendData("Data");
  element = doc.createElement(elementTagName);

  piData = pi.getData();
  length = piData.length();
  start = 0;
  firstQuoteFlag = 1;
  nameValuePairFlag = 0;
  for( i = 0, end = 0; i < length; i++, end++ ) {
    c = piData.charAt(i);

    //
    // Remove Excess Padding.  Excess Padding messes up
    // the element.setAttribute function.
    //
    if ((( c == ' ') || (c == '\n') || (c == '\t')) 
         && (nameValuePairFlag == 0)) {
      start++;
    }

    if ( c == '=') {
      name = piData.substringData(start, end-start);
      start = end +1;
      nameValuePairFlag++;
    } 
    if (c == '"') {
      if (firstQuoteFlag) {
        start = i +1;
        firstQuoteFlag = 0;
      } else {
        value = piData.substringData(start, end-start);
        nameValuePairFlag++;
        firstQuoteFlag = 1;
        start = end +1;
      }
    }
    if (nameValuePairFlag == 2) {
      element.setAttribute(name, value);
      nameValuePairFlag = 0;
      name.deleteData(0, name.length() - 1);
      value.deleteData(0, value.length() - 1);
    } else if (nameValuePairFlag > 2) {
      cerr << "The Processing Instruction Data is not in the correct form.\nThe correct form is pairs of the form : name = \"value\" \n";
    }

  }
  return ( element );
}  

XdmfInt32
XdmfDOM::GetNumberOfProcessingInstructions( ) {
  DOM_NodeList   nodeList;
  XdmfInt32  number;
  DOM_Document  doc;
  
  doc = this->parser->getDocument();
  nodeList = doc.getChildNodes();
  number = this->SearchDOM(nodeList, 0, DOM_Node::PROCESSING_INSTRUCTION_NODE);
  return( number );
}

XdmfInt32
XdmfDOM::GetNumberOfProcessingInstructions( XdmfString tagName ) {
  DOM_NodeList   nodeList;
  XdmfInt32  number;
  DOM_Document  doc;
  
  doc = this->parser->getDocument();
  nodeList = doc.getChildNodes();
  number = this->SearchDOM(nodeList, tagName, 0, DOM_Node::PROCESSING_INSTRUCTION_NODE);
  return( number );
}

XdmfInt32
XdmfDOM::GetNumberOfProcessingInstructions( DOM_Element elementNode ) {
  DOM_NodeList  nodeList;
  XdmfInt32  number;
  
  nodeList = elementNode.getChildNodes();
  number = this->SearchDOM(nodeList, 0, DOM_Node::PROCESSING_INSTRUCTION_NODE);
  return( number );
}

XdmfInt32
XdmfDOM::GetNumberOfProcessingInstructions( XdmfString tagName, DOM_Element elementNode ) {
  DOM_NodeList  nodeList;
  XdmfInt32  number;
  
  nodeList = elementNode.getChildNodes();
  number = this->SearchDOM(nodeList, tagName, 0, DOM_Node::PROCESSING_INSTRUCTION_NODE);
  return( number );
}

DOM_ProcessingInstruction
XdmfDOM::GetProcessingInstruction ( XdmfString tagName ) {
  DOM_ProcessingInstruction  pi;  
  DOM_Node       node;  
  XdmfInt32       index;  
  
  index = 0;
  node = 0;
  pi = this->GetProcessingInstruction(tagName, index, node);
  return(pi);
}

DOM_ProcessingInstruction
XdmfDOM::GetProcessingInstruction ( XdmfString tagName, XdmfInt32 index ) {
  DOM_ProcessingInstruction   pi;  
  DOM_Node       node;  
  
  node = 0;
  pi = this->GetProcessingInstruction(tagName, index, node);
  return(pi);
}

DOM_ProcessingInstruction
XdmfDOM::GetProcessingInstruction ( XdmfString tagName, XdmfInt32 index, DOM_Node node) {
  DOM_ProcessingInstruction  pi;
  DOM_NodeList      nodeList;
  DOM_Document      doc;

  doc = this->parser->getDocument();
  
  if (node.isNull()) {
    nodeList = doc.getChildNodes();
  } else {
    nodeList = node.getChildNodes();
  }
  pi = (DOM_ProcessingInstruction&)this->SearchDOM( nodeList, tagName, index, DOM_Node::PROCESSING_INSTRUCTION_NODE, 1 );

  return( pi );
}




DOMParser *
XdmfDOM::GetParser ( ) {
  return(this->parser);
}

XdmfInt32
XdmfDOM::Parse ( XdmfString xml ) {

  XdmfInt32     size;
  MemBufInputSource  *mem;

  size = strlen(xml);
  mem = new MemBufInputSource ( (unsigned char *)xml, size, 
              "xmlString", false );

  this->parser->parse(*mem);
  return(XDMF_SUCCESS);
}

XdmfString
XdmfDOM::Serialize ( ) {
  DOM_Document    doc;

  doc = this->parser->getDocument();

        if ( this->encodingName == 0 ) {
                DOMString encNameStr("UTF-8");
                DOM_Node aNode = doc.getFirstChild();
                if ( aNode.getNodeType() == DOM_Node::XML_DECL_NODE) {
                        DOMString aStr = ((DOM_XMLDecl &)aNode).getEncoding();
                        if (aStr != "") {
                                encNameStr = aStr;
                        }
                }
                unsigned int len = encNameStr.length();
                this->encodingName = new XMLCh[len+1];
                XMLString::copyNString(this->encodingName, encNameStr.rawBuffer(), len);
                this->encodingName[len] = 0;
        }
        this->formatter = new XMLFormatter(this->encodingName, this,
                                           XMLFormatter::NoEscapes, unRepFlags);

        *this << doc;
  this->xmlStream << endl;
        return( this->xmlStream.str() );

}

XdmfInt32
XdmfDOM::SearchDOM( DOM_NodeList nodeList, XdmfInt32 count, XdmfInt32 searchType ) {
        DOM_Node        node;
        DOM_NodeList    children;
  XdmfInt32  nodeType;
        XdmfInt32  i;
  
        for (i = 0; i < nodeList.getLength(); i++) {
                node = nodeList.item(i);
    nodeType = node.getNodeType();
    if ( nodeType == searchType ) {
      count++; 
    }
    children = node.getChildNodes();
    count = this->SearchDOM(children, count, searchType);
        }
  return( count );
}

XdmfInt32
XdmfDOM::SearchDOM( DOM_NodeList nodeList, XdmfString tagName, XdmfInt32 count, XdmfInt32 searchType ) {
        DOM_Node        node;
        DOM_NodeList    children;
        DOMString       nodeName;
        XdmfInt32       nodeType;
        XdmfInt32  i;
  
        for (i = 0; i < nodeList.getLength(); i++) {
                node = nodeList.item(i);
                nodeName = node.getNodeName();
    nodeType = node.getNodeType();
    if ((nodeType == searchType) && (nodeName.equals(tagName))) {
      count++;
    }
                children = node.getChildNodes();
                count = this->SearchDOM(children, tagName, count, searchType);
        }
  return( count );
}

DOM_Node
XdmfDOM::SearchDOM( DOM_NodeList nodeList, XdmfString tagName, XdmfInt32 index, XdmfInt32 searchType, XdmfInt32 resetFlag) {
        DOM_Node        node;
        DOM_Node        retNode;
        DOM_NodeList    children;
        DOMString       nodeName;
  XdmfInt32  nodeType;
        XdmfInt32  i;
  static int  count = 0;

  if (resetFlag) {
    count = 0;
  }
  
  retNode = 0;
        for (i = 0; i < nodeList.getLength(); i++) {
                node = nodeList.item(i);
                nodeName = node.getNodeName();
    nodeType = node.getNodeType();
    if ( nodeName.equals(tagName) && (nodeType == searchType) ) {
      if (index == count) {
        return( node );
      } else {
        count++;
      }
    }
    children = node.getChildNodes();
          retNode = this->SearchDOM(children, tagName, index, searchType, 0);
    if ( !retNode.isNull() ) {
      return( retNode );
    }
        }
  return( retNode );
}


// ---------------------------------------------------------------------------
//  ostream << DOM_Node   
//
//  Stream out a DOM node, and, recursively, all of its children. This
//  function is the heart of writing a DOM tree out as XML source. Give it
//  a document node and it will do the whole thing.
// ---------------------------------------------------------------------------
XdmfDOM&
XdmfDOM::operator<<( DOM_Node& toWrite )
{
    // Get the name and value out for convenience
    DOMString   nodeName = toWrite.getNodeName();
    DOMString   nodeValue = toWrite.getNodeValue();
    unsigned long lent = nodeValue.length();

    switch (toWrite.getNodeType())
    {
        case DOM_Node::TEXT_NODE:
        {
            this->formatter->formatBuf(nodeValue.rawBuffer(),
                                  lent, XMLFormatter::CharEscapes);
            break;
        }

        case DOM_Node::PROCESSING_INSTRUCTION_NODE :
        {
            *this->formatter << XMLFormatter::NoEscapes << gStartPI;
      *this + nodeName;
            if (lent > 0)
            {
                *this->formatter << chSpace;
    *this + nodeValue;
            }
            *this->formatter << XMLFormatter::NoEscapes << gEndPI;
            break;
        }


        case DOM_Node::DOCUMENT_NODE :
        {

            DOM_Node child = toWrite.getFirstChild();
            while( child != 0)
            {
                *this << child;
    this->xmlStream << endl;
                child = child.getNextSibling();
            }
            break;
        }


        case DOM_Node::ELEMENT_NODE :
        {
            // The name has to be representable without any escapes
            *this->formatter  << XMLFormatter::NoEscapes
                         << chOpenAngle;
      *this + nodeName;

            // Output the element start tag.

            // Output any attributes on this element
            DOM_NamedNodeMap attributes = toWrite.getAttributes();
            int attrCount = attributes.getLength();
            for (int i = 0; i < attrCount; i++)
            {
                DOM_Node  attribute = attributes.item(i);

                //
                //  Again the name has to be completely representable. But the
                //  attribute can have refs and requires the attribute style
                //  escaping.
                //
                *this->formatter  << XMLFormatter::NoEscapes
                             << chSpace;
    *this + attribute.getNodeName();
    *this->formatter << chEqual << chDoubleQuote
                             << XMLFormatter::AttrEscapes;
    *this + attribute.getNodeValue();
    *this->formatter << XMLFormatter::NoEscapes
                             << chDoubleQuote;
            }

            //
            //  Test for the presence of children, which includes both
            //  text content and nested elements.
            //
            DOM_Node child = toWrite.getFirstChild();
            if (child != 0)
            {
                // There are children. Close start-tag, and output children.
                // No escapes are legal here
                *this->formatter << XMLFormatter::NoEscapes << chCloseAngle;

                while( child != 0)
                {
                    *this << child;
                    child = child.getNextSibling();
                }

                //
                // Done with children.  Output the end tag.
                //
                *this->formatter << XMLFormatter::NoEscapes << gEndElement;
    *this + nodeName;
    *this->formatter << chCloseAngle;
            }
            else
            {
                //
                //  There were no children. Output the short form close of
                //  the element start tag, making it an empty-element tag.
                //
                *this->formatter << XMLFormatter::NoEscapes << chForwardSlash << chCloseAngle;
            }
            break;
        }


        case DOM_Node::ENTITY_REFERENCE_NODE:
            {
                DOM_Node child;
#if 0
                for (child = toWrite.getFirstChild();
                child != 0;
                child = child.getNextSibling())
                {
                    *this << child;
                }
#else
                //
                // Instead of printing the refernece tree
                // we'd output the actual text as it appeared in the xml file.
                // This would be the case when -e option was chosen
                //
                    *this->formatter << XMLFormatter::NoEscapes << chAmpersand;
        *this + nodeName;
        *this->formatter << chSemiColon;
#endif
                break;
            }


        case DOM_Node::CDATA_SECTION_NODE:
            {
            *this->formatter << XMLFormatter::NoEscapes << gStartCDATA;
      *this + nodeValue;
            *this->formatter << gEndCDATA;
            break;
        }


        case DOM_Node::COMMENT_NODE:
        {
            *this->formatter << XMLFormatter::NoEscapes << gStartComment;
      *this + nodeValue;
       *this->formatter << gEndComment;
            break;
        }


        case DOM_Node::DOCUMENT_TYPE_NODE:
        {
            DOM_DocumentType doctype = (DOM_DocumentType &)toWrite;;

            *this->formatter << XMLFormatter::NoEscapes  << gStartDoctype;
      *this + nodeName;
 
            DOMString id = doctype.getPublicId();
            if (id != 0)
            {
                *this->formatter << XMLFormatter::NoEscapes << chSpace << gPublic;
                *this + id;
    *this->formatter << chDoubleQuote;
                id = doctype.getSystemId();
                if (id != 0)
                {
                    *this->formatter << XMLFormatter::NoEscapes << chSpace
                       << chDoubleQuote;
        *this + id;
        *this->formatter << chDoubleQuote;
                }
            }
            else
            {
                id = doctype.getSystemId();
                if (id != 0)
                {
                    *this->formatter << XMLFormatter::NoEscapes << chSpace 
        << gSystem;
        *this + id;
        *this->formatter << chDoubleQuote;
                }
            }

            id = doctype.getInternalSubset();
            if (id !=0)
                *this->formatter << XMLFormatter::NoEscapes << chOpenSquare;
    *this + id;
    *this->formatter << chCloseSquare;

            *this->formatter << XMLFormatter::NoEscapes << chCloseAngle;
            break;
        }


        case DOM_Node::ENTITY_NODE:
        {
            *this->formatter << XMLFormatter::NoEscapes << gStartEntity;
      *this + nodeName;

            DOMString id = ((DOM_Entity &)toWrite).getPublicId();
            if (id != 0)
                *this->formatter << XMLFormatter::NoEscapes << gPublic;
    *this + id;
    *this->formatter << chDoubleQuote;

            id = ((DOM_Entity &)toWrite).getSystemId();
            if (id != 0)
                *this->formatter << XMLFormatter::NoEscapes << gSystem;
    *this + id;
    *this->formatter << chDoubleQuote;

            id = ((DOM_Entity &)toWrite).getNotationName();
            if (id != 0)
                *this->formatter << XMLFormatter::NoEscapes << gNotation;
    *this + id;
    *this->formatter << chDoubleQuote;

            *this->formatter << XMLFormatter::NoEscapes << chCloseAngle << chCR << chLF;

            break;
        }


        case DOM_Node::XML_DECL_NODE:
        {
            DOMString  str;

            *this->formatter << gXMLDecl1;
         *this + ((DOM_XMLDecl &)toWrite).getVersion();

            *this->formatter << gXMLDecl2 << this->encodingName;

            str = ((DOM_XMLDecl &)toWrite).getStandalone();
            if (str != 0)
                *this->formatter << gXMLDecl3;
    *this + str;

            *this->formatter << gXMLDecl4;

            break;
        }

        default:
            cerr << "Unrecognized node type = "
                 << (long)toWrite.getNodeType() << endl;
    }
    return *this;
}

// ---------------------------------------------------------------------------
//  ostream << DOMString
//
//  Stream out a DOM string. Doing this requires that we first transcode
//  to char * form in the default code page for the system
// ---------------------------------------------------------------------------
XdmfDOM& 
XdmfDOM::operator<< ( const DOMString& s )
{
    char *p = s.transcode();
    this->xmlStream << p;
    delete [] p;
    return *this;
}

XdmfDOM& 
XdmfDOM::operator+ ( const DOMString& s )
{
    unsigned int lent = s.length();

        if (lent <= 0)
                return *this;

    XMLCh*  buf = new XMLCh[lent + 1];
    XMLString::copyNString(buf, s.rawBuffer(), lent);
    buf[lent] = 0;
    *this->formatter << buf;
    delete [] buf;
    return *this;
}




