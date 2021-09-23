
:VOLU mesh_container BOX 10. 10. 10. G4_WATER
:PLACE mesh_container 1 world R00 22. 0. 0.
:VOLU mesh BOX 0.5 0.5 0.5 G4_WATER
:PLACE_PARAM mesh 1 mesh_container PHANTOM 20 20 20 1. 1. 1.
:PLACE mesh_container 1 R00 world 