bl_info = {
        "name": "Nut Exporter",
        "category": "Import-Export",
        "author": "Max Kemp"}
        

import bpy
from bpy.types import Panel
from bpy.types import Mesh
import os
import mathutils
from mathutils import *
from collections import OrderedDict
    

def main(context):
    fileName = bpy.path.basename(bpy.context.blend_data.filepath)
    fileName = fileName[:-6]
    testPath = os.path.join(os.environ["HOMEPATH"], "Desktop")
    
    deformBones = []
    bindPosePositions = {}
    rootBoneGlobalMat = Matrix()
    restBones = {}
    poseBones = {}
    armatureObject = bpy.data.objects["Armature"]
    
    
    # Set up file to export to
    path = "C:/Users/Maxke/Desktop/Test Exporter/" + fileName + "/"
    os.makedirs(path, exist_ok = True)
    
    with open(path + "Armature.nut", "w") as file:
        file.write("%s " % (testPath "/" + fileName + "/"))
        # ///////// Armature /////////
        # Write the joint hierarchy.
        obj = bpy.data.objects["Armature"]
        if obj.type == 'ARMATURE':
            armatureObject = obj
            # ///////// Bone hierarchy info /////////
            file.write("%s" % ("Bones"))
            file.write("%s" % ("\n"))
            boneIndex = 0
            for poseBone in obj.pose.bones:
                poseBones[poseBone.name] = poseBone
                
                bone = poseBone.bone
                
                # Only access deform bones. Ignore control bones.
                if bone.use_deform:
                    deformBones.append(bone.name)
                    
                    file.write("%i" % (boneIndex))
                    file.write(" %s" % ("\"" + bone.name + "\""))
                    
                    # Calculate parent bone indices
                    parentIndex = -1
                    # Not the root bone
                    if (bone.parent is not None):
                        parentBoneName = bone.parent.name
                        # Loop from the root bone till we find this bone's parent
                        for parentBone in obj.pose.bones:
                            # Only access deform bones. Ignore control bones.
                            if not parentBone.bone.use_deform:
                                continue;
                            
                            parentIndex = parentIndex + 1
                            if parentBone.name == parentBoneName:
                                break
                    
                    file.write(" %i" % (parentIndex))
                    file.write("%s" % ("\n"))
                    
                    boneIndex = boneIndex + 1
                    
            # Declare the end of the "Bones" section.
            file.write("%s" % ("end"))
            file.write("%s" % ("\n"))
            
            
            # ///////// Bind pose info /////////
            file.write("%s" % ("BindPose"))
            file.write("%s" % ("\n"))
            boneIndex = 0
            for editBone in obj.data.edit_bones:
                # Only access deform bones. Ignore control bones.
                if editBone.use_deform:
                    restBones[editBone.name] = editBone
                    file.write("%i" % (boneIndex))
                    boneIndex = boneIndex + 1
                    
                    # Extract local position.
                    localPos = Vector((0, 0, 0))
                    if (editBone.parent is None):
                        localPos = editBone.matrix.to_translation()
                    else:
                        #localPos = editBone.matrix.to_translation()
                        poseBone = poseBones[editBone.name]
                        localPos = armatureObject.convert_space(poseBone.parent, editBone.matrix, 'POSE', 'LOCAL').to_translation()
                        bindPosePositions[bone.name] = localPos
                    file.write("  %8.7f %8.7f %8.7f" % (localPos.x, localPos.y, localPos.z))
                    
                    # Extract local rotation
                    quat = editBone.matrix.to_quaternion()
                    if editBone.parent is not None:
                        poseBone = poseBones[editBone.name]
                        #quat = armatureObject.convert_space(poseBone.parent, editBone.matrix, 'POSE', 'LOCAL').to_quaternion()
                        quat = poseBone.bone.matrix.to_quaternion().normalized()
                    file.write("  %8.7f %8.7f %8.7f %8.7f" % (quat.w, quat.x, quat.y, quat.z))
                    #euler = poseBone.rotation_quaternion.to_euler()
                    #file.write("  %8.7f %8.7f %8.7f" % (euler.x, euler.y, euler.z))
                    file.write("%s" % ("\n"))
            
            # Declare the end of the "BindPose" section.
            file.write("%s" % ("end"))
            file.write("%s" % ("\n"))
                    
                    
        # ///////// Mesh /////////
        # Write initial bind pose.
        # Write each polygon (vertices, normals, uvs, joint ids, and joint weights.
        obj = bpy.data.objects["Cube"]           
        if obj.type == 'MESH':
            # Access mesh data
            mesh = obj.data
            vertices = mesh.vertices
            uv_layer = mesh.uv_layers.active.data
            
            # Declare the end of the "Bones" section.
            file.write("%s" % ("Triangles"))
            file.write("%s" % ("\n"))
            
            # Access each polygon (triangle)
            for poly in mesh.polygons:
                file.write("%s %i" % ("Tri:", poly.index))
                file.write("%s" % ("\n"))
                # Access each vertex of the current polygon (triangle)
                for loop_index in range(poly.loop_start, poly.loop_start + poly.loop_total):
                    # Vertex, normal, and uv
                    vertexID = mesh.loops[loop_index].vertex_index
                    meshVertex = vertices[vertexID]
                    vertex = meshVertex.co
                    normal = meshVertex.normal
                    uv = uv_layer[loop_index].uv
                    
                    file.write("%i" % (vertexID))
                    file.write("  %8.7f %8.7f %8.7f" % (vertex.x, vertex.y, vertex.z))
                    file.write("  %8.7f %8.7f %8.7f" % (normal.x, normal.y, normal.z))
                    file.write("  %8.7f %8.7f" % (uv.x, uv.y))
                    
                    # Joints deforming vertices, with their weights
                    file.write("  %i" % (len(meshVertex.groups)))
                    for group in meshVertex.groups:
                        file.write(" %i" % (group.group))
                        file.write(" %8.7f" % (group.weight))
                    
                    file.write("%s" % ("\n"))
                    
            # Declare the end of the "Triangle" section.
            file.write("%s" % ("end"))
            
    # Close the file                
    file.close()
    
    # Get the fps of the scene
    fps = context.scene.render.fps
    # Save each animation in a seperate file
    for action in bpy.data.actions:
        # Set up file to export to
        path = "C:/Users/Maxke/Desktop/Test Exporter/" + fileName + "/Anims/"
        os.makedirs(path, exist_ok = True)
        
        with open(path + action.name + ".nutAnim", "w") as file:
            # ///////// Animation ///////// 
            
            # Get each bones animated properties (fcurves for Position for example)
            for actionGroup in action.groups:
                # Only access deform bones. Ignore control bones.
                if actionGroup.name not in deformBones:
                    continue;
                
                allKeyFrames = OrderedDict()
                
                # Each property's individual component's fcurve (Position.x for example)
                for fcurve in actionGroup.channels:
                    #file.write("%s %i" % ((fcurve.data_path + " channel "), fcurve.array_index))
                    #file.write("%s" % ("\n"))
                    # Key frame time and value of the current component (time: 0, value: 0.35 for example)
                    for keyFrame in fcurve.keyframe_points:
                        pos = keyFrame.co
                        #file.write("  %8.7f %8.7f" % (pos.x, pos.y))
                        #file.write("%s" % ("\n"))
                        
                        if pos.x in allKeyFrames:
                            allKeyFrames[pos.x].append(pos.y)
                        else:
                            allKeyFrames[pos.x] = [pos.y]
                        
                        
                    #file.write("%s" % ("\n"))
                
                # Write this joint's frames to the file
                file.write("%s" % (actionGroup.name))
                file.write("%s" % ("\n"))
                for time in allKeyFrames:
                    # All joint frames
                    frames = allKeyFrames[time]
                    
                     # Calculate the time in seconds, for this frame
                    file.write("%8.7f" % (time / fps))
                    
                    
                    # Position channel
                    location = Vector((frames[0], frames[1], frames[2]))
                    
                    restBone = restBones[actionGroup.name]
                    pos = Vector((0, 0, 0))
                    if restBone.parent is None:
                        poseBone = poseBones[actionGroup.name]
                        pos = poseBone.matrix * location
                    else:
                        poseBone = poseBones[actionGroup.name]
                        #pos = poseBone.matrix * location
                        pos = armatureObject.convert_space(poseBone.parent, poseBone.matrix, 'POSE', 'LOCAL').to_translation()
                        #pos = poseBone.bone.matrix_local.to_translation()
                        
                    file.write("  %8.7f %8.7f %8.7f" % (pos.x, pos.y, pos.z))
                    
                        
                    # Quaternion channel
                    if restBone.parent is None:
                        quat = Quaternion((frames[3], frames[4], frames[5], frames[6]))
                        poseBoneQuat = poseBones[actionGroup.name].matrix.to_quaternion()
                        newPosQuat =  poseBoneQuat.normalized() * quat.normalized()
                        file.write("  %8.7f %8.7f %8.7f %8.7f" % (newPosQuat.w, newPosQuat.x, newPosQuat.y, newPosQuat.z))
                        file.write("%s" % ("\n"))
                    else:
                        poseBone = poseBones[actionGroup.name]
                        quat = Quaternion((frames[3], frames[4], frames[5], frames[6]))
#                        quatMat = quat.to_matrix().to_4x4()
#                        quatMat[3][3] = 1
#                        newPoseMatrix = poseBone.matrix * quatMat
                        newPoseMatrix = (poseBone.matrix.to_quaternion().normalized() * quat.normalized()).normalized().to_matrix().to_4x4()
                        newPosQuat = armatureObject.convert_space(poseBone.parent, newPoseMatrix, 'POSE', 'LOCAL').to_quaternion()
                        file.write("  %8.7f %8.7f %8.7f %8.7f" % (newPosQuat.w, newPosQuat.x, newPosQuat.y, newPosQuat.z))
                        file.write("%s" % ("\n"))
                    
                # Declare the end of this joint's frames section.
                file.write("%s" % ("end"))
                file.write("%s" % ("\n"))
                
            # Declare the end of this file.
            file.write("%s" % ("eof"))
        


class SaveFile(bpy.types.Operator):
    """Saves my custom file"""
    bl_idname = "wm.save"
    bl_label = "Save"
    #bl_options = {'REGISTER'}
    
    def execute(self, context):
        #print("Saving")
        main(context)
        self.report({'INFO'}, "File Nutted ;)")
        
        return {'FINISHED'}


class CustomExportPanel(Panel):
    """Creates a Panel in the Object properties window"""
    bl_label = "Nut Export Panel"
    bl_idname = "OBJECT_PT_hello"
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "object"
    bl_options = {'DEFAULT_CLOSED'}
    

    def draw(self, context):
        layout = self.layout
        
        row = layout.row()
        row.operator("wm.save")
        
        # Access currenly selected object's data.
        #obj = context.object

        # ///////// Armature /////////
        obj = bpy.data.objects["Armature"]
        if obj.type == 'ARMATURE':
            # Access all bones
            #for bone in armature.bones:
            for poseBone in obj.pose.bones:
                bone = poseBone.bone
                
                # Only access deform bones. Ignore control bones.
                if bone.use_deform:
                    
                    box = layout.box()
                    box.label("Rest Bone:" + bone.name)
                    
                    # ///////// Rest bone info /////////
                    column = box.column(True)
                    column.label(text="Deform: " + str(bone.use_deform))
                    column.label(text = "Bone Mat: " + str(bone.matrix_local))
                    column.label(text = "Bone Mat: " + str(bone.matrix))
                    # Extract local position
                    if (bone.parent is None):
                        column.label(text = "Pos: " + str(bone.head))
                    else:
                        column.label(text = "Pos: " + str(bone.head_local - bone.parent.head_local))
                    column.label(text = "Euler: " + str(bone.matrix_local.to_euler()))
                    column.label(text = "Quat: " + str(bone.matrix_local.to_quaternion()))
                    
                    # /////////////// REST TEST
                    column.label(text = "")
                    editBones = obj.data.edit_bones
                    #editBone = editBones[poseBone.name]
                    for eb in editBones:
                        if poseBone.name == eb.name:
                            column.label(text = eb.name)
                            if eb.parent is not None:
                                column.label(text = "Rest: " + str((eb.matrix).to_quaternion()))
                                column.label(text = "Pose: " + str((poseBone.matrix).to_quaternion()))
                                ebInv = eb.matrix.copy()
                                ebInv.invert()
                                #column.label(text = "F: " + str((poseBone.matrix * ebInv).to_quaternion()))
                                column.label(text = "F: " + str((poseBone.matrix_basis).to_quaternion()))
                                column.label(text = "F: " + str((poseBone.matrix_basis).to_euler()))
                            else:
                                column.label(text = "Rest: " + str((eb.matrix)))
                                column.label(text = "Pose: " + str((obj.matrix_world * poseBone.matrix).to_euler()))
                                ebInv = eb.matrix.copy()
                                ebInv.invert()
                                column.label(text = "F: " + str((poseBone.matrix * ebInv).to_euler()))

                            
                    column.label(text = "TEST: " + str((bone.matrix).to_euler()))
                    column.label(text = "")
                    
                    # ///////// Pose bone info /////////
                    column.label(text = "Pose:")
                    column.label(text = "Loc: " + str(poseBone.location))
                     # Extract local position
                    if (bone.parent is None):
                        column.label(text = "Pos: " + str(poseBone.head))
                    else:
                        column.label(text = "Pos: " + str(poseBone.head - poseBone.parent.head))
                    column.label(text = "Global Pos: " + str(obj.matrix_world * poseBone.matrix * poseBone.location))
                    column.label(text = "Quat: " + str(poseBone.rotation_quaternion))
                    column.label(text = "Quat Euler: " + str(poseBone.rotation_quaternion.to_euler()))
               
                   
        # ///////// Mesh /////////
#        obj = bpy.data.objects["Cube"]           
#        if obj.type == 'MESH':            
#            # Access mesh data
#            mesh = obj.data
#            vertices = mesh.vertices
#            uv_layer = mesh.uv_layers.active.data
#            
#            # Access each polygon (triangle)
#            i = 127
#            while i < 150:
#                poly = mesh.polygons[i]
#                i = i + 1
#                #for poly in mesh.polygons:
#                box = layout.box()
#                column = box.column(True)
#                column.label(text = "Poly Index: " + str(poly.index))
#                column.label(text = "Poly Length: " + str(poly.loop_total))
#                
#                # Access each vertex of the current polygon (triangle)
#                for loop_index in range(poly.loop_start, poly.loop_start + poly.loop_total):
#                    box = layout.box()
#                    column = box.column(True)
#                    
#                    # Vertex, normal, and uv
#                    vertexID = mesh.loops[loop_index].vertex_index
#                    meshVertex = vertices[vertexID]
#                    vertex = meshVertex.co
#                    column.label(text = "       Vertex ID: " + str(vertexID))
#                    column.label(text = "       Vertex: " + str(vertex))
#                    column.label(text = "       Normal: " + str(meshVertex.normal))
#                    column.label(text = "       UV: " + str(uv_layer[loop_index].uv))
#                    
#                    # Joints deforming vertices, with their weights
#                    for group in meshVertex.groups:
#                        column.label(text = "       Group: " + str(group.group))
#                        column.label(text = "       Weight: " + str(group.weight))



def register():
    bpy.utils.register_class(SaveFile)
    bpy.utils.register_class(CustomExportPanel)


def unregister():
    bpy.utils.unregister_class(SaveFile)
    bpy.utils.unregister_class(CustomExportPanel)


if __name__ == "__main__":
    register()