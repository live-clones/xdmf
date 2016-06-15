! Function Return Types
        integer xdmfaddattribute
        integer xdmfaddinformation
        integer xdmfsetgeometry
        integer xdmfsettopology
        integer xdmfsetdimensions
        integer xdmfsetorigin
        integer xdmfsetbrick
        integer xdmfstoremap
        integer xdmfaddcoordinate
        integer xdmfaddset
        integer xdmfgetmaxopenedfiles
        integer xdmfaddfunctionasattribute
        integer xdmfsetfunctionasgeometry
        integer xdmfsetfunctionastopology
        integer xdmfaddfunctionascoordinate
        integer xdmfaddfunctionasset
        integer xdmfaddsubsetasattribute
        integer xdmfsetsubsetasgeometry
        integer xdmfsetsubsetastopology
        integer xdmfaddsubsetascoordinate
        integer xdmfaddsubsetasset

! Array Type
        integer XDMF_ARRAY_TYPE_INT8
        integer XDMF_ARRAY_TYPE_INT16
        integer XDMF_ARRAY_TYPE_INT32
        integer XDMF_ARRAY_TYPE_INT64
        integer XDMF_ARRAY_TYPE_UINT8
        integer XDMF_ARRAY_TYPE_UINT16
        integer XDMF_ARRAY_TYPE_UINT32
        integer XDMF_ARRAY_TYPE_FLOAT32
        integer XDMF_ARRAY_TYPE_FLOAT64

! Attribute Center
        integer XDMF_ATTRIBUTE_CENTER_GRID
        integer XDMF_ATTRIBUTE_CENTER_CELL
        integer XDMF_ATTRIBUTE_CENTER_FACE
        integer XDMF_ATTRIBUTE_CENTER_EDGE
        integer XDMF_ATTRIBUTE_CENTER_NODE

! Attribute Type
        integer XDMF_ATTRIBUTE_TYPE_SCALAR
        integer XDMF_ATTRIBUTE_TYPE_VECTOR
        integer XDMF_ATTRIBUTE_TYPE_TENSOR
        integer XDMF_ATTRIBUTE_TYPE_MATRIX
        integer XDMF_ATTRIBUTE_TYPE_TENSOR6
        integer XDMF_ATTRIBUTE_TYPE_GLOBALID
        integer XDMF_ATTRIBUTE_TYPE_NOTYPE

! Geometry Type
        integer XDMF_GEOMETRY_TYPE_XYZ
        integer XDMF_GEOMETRY_TYPE_XY
        integer XDMF_GEOMETRY_TYPE_POLAR
        integer XDMF_GEOMETRY_TYPE_SPHERICAL

! Grid Collection Type
        integer XDMF_GRID_COLLECTION_TYPE_SPATIAL
        integer XDMF_GRID_COLLECTION_TYPE_TEMPORAL

! Topology Type
        integer XDMF_TOPOLOGY_TYPE_POLYVERTEX
        integer XDMF_TOPOLOGY_TYPE_POLYLINE
        integer XDMF_TOPOLOGY_TYPE_POLYGON
        integer XDMF_TOPOLOGY_TYPE_TRIANGLE
        integer XDMF_TOPOLOGY_TYPE_QUADRILATERAL
        integer XDMF_TOPOLOGY_TYPE_TETRAHEDRON
        integer XDMF_TOPOLOGY_TYPE_PYRAMID
        integer XDMF_TOPOLOGY_TYPE_WEDGE
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON
        integer XDMF_TOPOLOGY_TYPE_POLYHEDRON
        integer XDMF_TOPOLOGY_TYPE_EDGE_3
        integer XDMF_TOPOLOGY_TYPE_TRIANGLE_6
        integer XDMF_TOPOLOGY_TYPE_QUADRILATERAL_8
        integer XDMF_TOPOLOGY_TYPE_QUADRILATERAL_9
        integer XDMF_TOPOLOGY_TYPE_TETRAHEDRON_10
        integer XDMF_TOPOLOGY_TYPE_PYRAMID_13
        integer XDMF_TOPOLOGY_TYPE_WEDGE_15
        integer XDMF_TOPOLOGY_TYPE_WEDGE_18
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_20
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_24
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_27
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_64
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_125
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_216
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_343
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_512
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_729
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_1000
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_1331
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_64
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_125
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_216
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_343
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_512
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_729
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_1000
        integer XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_1331
        integer XDMF_TOPOLOGY_TYPE_MIXED

! Set Type
        integer XDMF_SET_TYPE_NODE
        integer XDMF_SET_TYPE_CELL
        integer XDMF_SET_TYPE_FACE
        integer XDMF_SET_TYPE_EDGE

! Grid Type
        integer XDMF_GRID_TYPE_CURVILINEAR
        integer XDMF_GRID_TYPE_RECTILINEAR
        integer XDMF_GRID_TYPE_REGULAR
        integer XDMF_GRID_TYPE_UNSTRUCTURED

! Binary Endian
        integer XDMF_BINARY_ENDIAN_NATIVE
        integer XDMF_BINARY_ENDIAN_LITTLE
        integer XDMF_BINARY_ENDIAN_BIG

!------------------------------------------------------

        parameter (XDMF_ARRAY_TYPE_INT8    = 0)
        parameter (XDMF_ARRAY_TYPE_INT16   = 1)
        parameter (XDMF_ARRAY_TYPE_INT32   = 2)
        parameter (XDMF_ARRAY_TYPE_INT64   = 3)
        parameter (XDMF_ARRAY_TYPE_UINT8   = 4)
        parameter (XDMF_ARRAY_TYPE_UINT16  = 5)
        parameter (XDMF_ARRAY_TYPE_UINT32  = 6)
        parameter (XDMF_ARRAY_TYPE_FLOAT32 = 7)
        parameter (XDMF_ARRAY_TYPE_FLOAT64 = 8)

        parameter (XDMF_ATTRIBUTE_CENTER_GRID = 100)
        parameter (XDMF_ATTRIBUTE_CENTER_CELL = 101)
        parameter (XDMF_ATTRIBUTE_CENTER_FACE = 102)
        parameter (XDMF_ATTRIBUTE_CENTER_EDGE = 103)
        parameter (XDMF_ATTRIBUTE_CENTER_NODE = 104)

        parameter (XDMF_ATTRIBUTE_TYPE_SCALAR   = 200)
        parameter (XDMF_ATTRIBUTE_TYPE_VECTOR   = 201)
        parameter (XDMF_ATTRIBUTE_TYPE_TENSOR   = 202)
        parameter (XDMF_ATTRIBUTE_TYPE_MATRIX   = 203)
        parameter (XDMF_ATTRIBUTE_TYPE_TENSOR6  = 204)
        parameter (XDMF_ATTRIBUTE_TYPE_GLOBALID = 205)
        parameter (XDMF_ATTRIBUTE_TYPE_NOTYPE   = 206)

        parameter (XDMF_GEOMETRY_TYPE_XYZ       = 301)
        parameter (XDMF_GEOMETRY_TYPE_XY        = 302)
        parameter (XDMF_GEOMETRY_TYPE_POLAR     = 303)
        parameter (XDMF_GEOMETRY_TYPE_SPHERICAL = 304)

        parameter (XDMF_GRID_COLLECTION_TYPE_SPATIAL  = 400)
        parameter (XDMF_GRID_COLLECTION_TYPE_TEMPORAL = 401)

        parameter (XDMF_TOPOLOGY_TYPE_POLYVERTEX               = 500)
        parameter (XDMF_TOPOLOGY_TYPE_POLYLINE                 = 501)
        parameter (XDMF_TOPOLOGY_TYPE_POLYGON                  = 502)
        parameter (XDMF_TOPOLOGY_TYPE_POLYHEDRON               = 503)
        parameter (XDMF_TOPOLOGY_TYPE_TRIANGLE                 = 504)
        parameter (XDMF_TOPOLOGY_TYPE_QUADRILATERAL            = 505)
        parameter (XDMF_TOPOLOGY_TYPE_TETRAHEDRON              = 506)
        parameter (XDMF_TOPOLOGY_TYPE_PYRAMID                  = 507)
        parameter (XDMF_TOPOLOGY_TYPE_WEDGE                    = 508)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON               = 509)
        parameter (XDMF_TOPOLOGY_TYPE_EDGE_3                   = 510)
        parameter (XDMF_TOPOLOGY_TYPE_TRIANGLE_6               = 511)
        parameter (XDMF_TOPOLOGY_TYPE_QUADRILATERAL_8          = 512)
        parameter (XDMF_TOPOLOGY_TYPE_QUADRILATERAL_9          = 513)
        parameter (XDMF_TOPOLOGY_TYPE_TETRAHEDRON_10           = 514)
        parameter (XDMF_TOPOLOGY_TYPE_PYRAMID_13               = 515)
        parameter (XDMF_TOPOLOGY_TYPE_WEDGE_15                 = 516)
        parameter (XDMF_TOPOLOGY_TYPE_WEDGE_18                 = 517)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_20            = 518)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_24            = 519)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_27            = 520)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_64            = 521)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_125           = 522)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_216           = 523)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_343           = 524)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_512           = 525)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_729           = 526)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_1000          = 527)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_1331          = 528)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_64   = 529)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_125  = 530)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_216  = 531)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_343  = 532)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_512  = 533)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_729  = 534)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_1000 = 535)
        parameter (XDMF_TOPOLOGY_TYPE_HEXAHEDRON_SPECTRAL_1331 = 536)
        parameter (XDMF_TOPOLOGY_TYPE_MIXED                    = 537)

        parameter (XDMF_SET_TYPE_NODE = 601)
        parameter (XDMF_SET_TYPE_CELL = 602)
        parameter (XDMF_SET_TYPE_FACE = 603)
        parameter (XDMF_SET_TYPE_EDGE = 604)

        parameter (XDMF_GRID_TYPE_CURVILINEAR   = 701)
        parameter (XDMF_GRID_TYPE_RECTILINEAR   = 702)
        parameter (XDMF_GRID_TYPE_REGULAR       = 703)
        parameter (XDMF_GRID_TYPE_UNSTRUCTURED  = 704)

        parameter (XDMF_BINARY_ENDIAN_NATIVE = 801)
        parameter (XDMF_BINARY_ENDIAN_LITTLE = 802)
        parameter (XDMF_BINARY_ENDIAN_BIG    = 803)
