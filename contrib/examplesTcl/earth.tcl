catch {load vtktcl}
if { [catch {set VTK_TCL $env(VTK_TCL)}] != 0} { set VTK_TCL "../../examplesTcl" }
if { [catch {set VTK_DATA $env(VTK_DATA)}] != 0} { set VTK_DATA "../../../vtkdata" }

# this is a tcl version of the Mace example
# get the interactor ui
source $VTK_TCL/vtkInt.tcl

# Create the RenderWindow, Renderer and both Actors
#
vtkRenderer ren1
vtkRenderWindow renWin
    renWin AddRenderer ren1
vtkRenderWindowInteractor iren
    iren SetRenderWindow renWin

vtkTexturedSphereSource tss
tss SetThetaResolution 18
tss SetPhiResolution 9
vtkPolyDataMapper   earthMapper
earthMapper SetInput [tss GetOutput]
vtkActor earthActor
earthActor SetMapper earthMapper

# load in the texture map
#
vtkTexture atext
vtkPNMReader pnmReader
pnmReader SetFileName "$VTK_DATA/earth.ppm"
atext SetInput [pnmReader GetOutput]
atext InterpolateOn
earthActor SetTexture atext

# create a earth source and actor
#
vtkEarthSource es
es SetRadius 0.501
es SetOnRatio 2
vtkPolyDataMapper   earth2Mapper
earth2Mapper SetInput [es GetOutput]
vtkActor earth2Actor
earth2Actor SetMapper earth2Mapper

# Add the actors to the renderer, set the background and size
#
ren1 AddActor earthActor
ren1 AddActor earth2Actor
ren1 SetBackground 0 0 0.1
renWin SetSize 300 300

# render the image
#
iren SetUserMethod {wm deiconify .vtkInteract}
set cam1 [ren1 GetActiveCamera]
$cam1 Zoom 1.4
iren Initialize
#renWin SetFileName "earth.tcl.ppm"
#renWin SaveImageAsPPM

# prevent the tk window from showing up then start the event loop
wm withdraw .


