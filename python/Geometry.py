#/*******************************************************************/
#/*                               XDMF                              */
#/*                   eXtensible Data Model and Format              */
#/*                                                                 */
#/*  Id : Id  */
#/*  Date : $Date$ */
#/*  Version : $Revision$ */
#/*                                                                 */
#/*  Author:                                                        */
#/*     Jerry A. Clarke                                             */
#/*     clarke@arl.army.mil                                         */
#/*     US Army Research Laboratory                                 */
#/*     Aberdeen Proving Ground, MD                                 */
#/*                                                                 */
#/*     Copyright @ 2002 US Army Research Laboratory                */
#/*     All Rights Reserved                                         */
#/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
#/*                                                                 */
#/*     This software is distributed WITHOUT ANY WARRANTY; without  */
#/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
#/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
#/*     for more information.                                       */
#/*                                                                 */
#/*******************************************************************/
"""

Displays Geometry input

"""

__author__ = "Jerry Clarke"
__version__ = "$Revision: 1.1 $"
__date__ = "$Date: 2002-12-02 17:12:20 $"

import Base.Objects, Common
import Tkinter, tkColorChooser
import vtkpython
import vtkPipeline.vtkMethodParser

debug = Common.debug

class Geometry (Base.Objects.Module):
    """ Displays polydata input, nothing fancy."""
    def __init__ (self, mod_m):
        debug ("In Geometry::__init__ ()")
        Common.state.busy ()
        Base.Objects.Module.__init__ (self, mod_m)
        self.act = None
        data_src = self.mod_m.get_data_source ()
	self.type = data_src.get_grid_type ()
        self.root = None
	self.geom = vtkpython.vtkGeometryFilter()
        self.mapper = self.map = vtkpython.vtkPolyDataMapper ()
        self.geom.SetInput (mod_m.GetOutput ())
        self.map.SetInput (self.geom.GetOutput())
        self.map.SetLookupTable (self.mod_m.get_scalar_lut ())
        self.data_range = self.mod_m.get_scalar_data_range ()
        self.map.SetScalarRange (self.data_range)
        self.actor = self.act = vtkpython.vtkActor ()  
        self.act.SetMapper (self.map)
        apply (self.act.GetProperty ().SetColor, (0.0, 0.1, 0.8))
        # used for the pipeline browser
        self.pipe_objs = self.act
        self.renwin.add_actors (self.act)
        self.renwin.Render ()
        Common.state.idle ()

    def __del__ (self): 
        debug ("In Geometry::__del__ ()")
        if self.act:
            self.renwin.remove_actors (self.act)
        self.renwin.Render ()

    def SetInput (self, source): 
        debug ("In Geometry::SetInput ()")
        Common.state.busy ()
        self.map.SetInput (source)
        dr = self.mod_m.get_scalar_data_range ()
        if (dr[0] != self.data_range[0]) or (dr[1] != self.data_range[1]):
            self.map.SetScalarRange (dr)
            self.data_range = dr
        Common.state.idle ()

    def save_config (self, file): 
        debug ("In Geometry::save_config ()")
        p = vtkPipeline.vtkMethodParser.VtkPickler ()
        for obj in (self.map, self.act, self.act.GetProperty ()):
            p.dump (obj, file)

    def load_config (self, file): 
        debug ("In Geometry::load_config ()")
        p = vtkPipeline.vtkMethodParser.VtkPickler ()
        for obj in (self.map, self.act, self.act.GetProperty ()):
            p.load (obj, file)
        
    def config_changed (self): 
        debug ("In Geometry::config_changed ()")
        apply (self.act.GetProperty ().SetColor, Common.config.fg_color)

    def make_main_gui (self, master=None): 
        debug ("In Geometry::make_main_gui ()")
        "Create the GUI configuration controls for this object."
        self.make_actor_gui ()

