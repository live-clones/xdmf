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
#define XDMF_ARRAY_IN  0
#define XDMF_ARRAY_OUT  1

#define XDMF_ARRAY_COPY(ArrayPointer, ArrayType, ArrayStride, ValuePointer, ValueType, ValueStride, Direction, NumberOfValues  ) \
    switch(ArrayType) { \
      case XDMF_INT8_TYPE : \
        { \
        XdmfInt64   i; \
        XdmfInt8  *ap; \
         \
        i = NumberOfValues; \
        ap = (XdmfInt8 *)ArrayPointer; \
        if( Direction == XDMF_ARRAY_IN ) { \
          while(i--){ \
            *ap = *ValuePointer; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } else { \
          while(i--){ \
            *ValuePointer = *ap; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } \
        } \
        break; \
      case XDMF_INT32_TYPE : \
        { \
        XdmfInt64   i; \
        XdmfInt32  *ap; \
         \
        i = NumberOfValues; \
        ap = (XdmfInt32 *)ArrayPointer; \
        if( Direction == XDMF_ARRAY_IN ) { \
          while(i--){ \
            *ap = *ValuePointer; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } else { \
          while(i--){ \
            *ValuePointer = *ap; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } \
        } \
        break; \
      case XDMF_INT64_TYPE : \
        { \
        XdmfInt64   i; \
        XdmfInt64   *ap; \
         \
        i = NumberOfValues; \
        ap = (XdmfInt64 *)ArrayPointer; \
        if( Direction == XDMF_ARRAY_IN ) { \
          while(i--){ \
            *ap = *ValuePointer; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } else { \
          while(i--){ \
            *ValuePointer = *ap; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } \
        } \
        break; \
      case XDMF_FLOAT32_TYPE : \
        { \
        XdmfInt64   i; \
        XdmfFloat32  *ap; \
         \
        i = NumberOfValues; \
        ap = (XdmfFloat32 *)ArrayPointer; \
        if( Direction == XDMF_ARRAY_IN ) { \
          while(i--){ \
            *ap = *ValuePointer; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } else { \
          while(i--){ \
            *ValuePointer = *ap; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } \
        } \
        break; \
      case XDMF_FLOAT64_TYPE : \
        { \
        XdmfInt64   i; \
        XdmfFloat64  *ap; \
         \
        i = NumberOfValues; \
        ap = (XdmfFloat64 *)ArrayPointer; \
        if( Direction == XDMF_ARRAY_IN ) { \
          while(i--){ \
            *ap = *ValuePointer; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } else { \
          while(i--){ \
            *ValuePointer = *ap; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } \
        } \
        break; \
      default : \
        this->CopyCompound(ArrayPointer, ArrayType, ArrayStride, \
          ValuePointer, ValueType, ValueStride, \
          Direction, NumberOfValues ); \
        break; \
      }


#define XDMF_ARRAY_OPERATE(OPERATOR, ArrayPointer, ArrayType, ArrayStride, ValuePointer, ValueType, ValueStride, Direction, NumberOfValues  ) \
    switch(ArrayType) { \
      case XDMF_INT8_TYPE : \
        { \
        XdmfInt64   i; \
        XdmfInt8  *ap; \
         \
        i = NumberOfValues; \
        ap = (XdmfInt8 *)ArrayPointer; \
        if( Direction == XDMF_ARRAY_IN ) { \
          while(i--){ \
            *ap OPERATOR  *ValuePointer; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } else { \
          while(i--){ \
            *ValuePointer OPERATOR  *ap; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } \
        } \
        break; \
      case XDMF_INT32_TYPE : \
        { \
        XdmfInt64   i; \
        XdmfInt32  *ap; \
         \
        i = NumberOfValues; \
        ap = (XdmfInt32 *)ArrayPointer; \
        if( Direction == XDMF_ARRAY_IN ) { \
          while(i--){ \
            *ap OPERATOR  *ValuePointer; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } else { \
          while(i--){ \
            *ValuePointer OPERATOR  *ap; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } \
        } \
        break; \
      case XDMF_INT64_TYPE : \
        { \
        XdmfInt64   i; \
        XdmfInt64   *ap; \
         \
        i = NumberOfValues; \
        ap = (XdmfInt64 *)ArrayPointer; \
        if( Direction == XDMF_ARRAY_IN ) { \
          while(i--){ \
            *ap OPERATOR  *ValuePointer; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } else { \
          while(i--){ \
            *ValuePointer OPERATOR  *ap; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } \
        } \
        break; \
      case XDMF_FLOAT32_TYPE : \
        { \
        XdmfInt64   i; \
        XdmfFloat32  *ap; \
         \
        i = NumberOfValues; \
        ap = (XdmfFloat32 *)ArrayPointer; \
        if( Direction == XDMF_ARRAY_IN ) { \
          while(i--){ \
            *ap OPERATOR  *ValuePointer; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } else { \
          while(i--){ \
            *ValuePointer OPERATOR  *ap; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } \
        } \
        break; \
      case XDMF_FLOAT64_TYPE : \
        { \
        XdmfInt64   i; \
        XdmfFloat64  *ap; \
         \
        i = NumberOfValues; \
        ap = (XdmfFloat64 *)ArrayPointer; \
        if( Direction == XDMF_ARRAY_IN ) { \
          while(i--){ \
            *ap OPERATOR  *ValuePointer; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } else { \
          while(i--){ \
            *ValuePointer OPERATOR  *ap; \
            ap += ArrayStride; \
            ValuePointer += ValueStride; \
          } \
        } \
        } \
        break; \
      default : \
        XdmfErrorMessage("Can't Assign Values to Compound Type"); \
        break; \
      }
