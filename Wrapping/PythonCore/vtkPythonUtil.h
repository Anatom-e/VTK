/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPythonUtil.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// .NAME vtkPythonUtil

#ifndef vtkPythonUtil_h
#define vtkPythonUtil_h

#include "vtkPython.h"
#include "PyVTKClass.h"
#include "PyVTKMutableObject.h"
#include "PyVTKNamespace.h"
#include "PyVTKObject.h"
#include "PyVTKSpecialObject.h"

class vtkPythonClassMap;
class vtkPythonCommand;
class vtkPythonCommandList;
class vtkPythonGhostMap;
class vtkPythonObjectMap;
class vtkPythonSpecialTypeMap;
class vtkPythonNamespaceMap;
class vtkStdString;
class vtkUnicodeString;
class vtkVariant;

extern "C" void vtkPythonUtilDelete();

class VTKWRAPPINGPYTHONCORE_EXPORT vtkPythonUtil
{
public:

  // Description:
  // If the name is templated or mangled, converts it into
  // a python-printable name.
  static const char *PythonicClassName(const char *classname);

  // Description:
  // Add a PyVTKClass to the type lookup table, this allows us to later
  // create object given only the class name.
  static void AddClassToMap(PyObject *obj, const char *classname);

  // Description:
  // Get information about a special VTK type, given the type name.
  static PyObject *FindClass(const char *classname);

  // Description:
  // For an VTK object whose class is not in the ClassMap, search
  // the whole ClassMap to find out which class is the closest base
  // class of the object.  Returns a PyVTKClass.
  static PyObject *FindNearestBaseClass(vtkObjectBase *ptr);

  // Description:
  // Extract the vtkObjectBase from a PyVTKObject.  If the PyObject is
  // not a PyVTKObject, or is not a PyVTKObject of the specified type,
  // the python error indicator will be set.
  // Special behavior: Py_None is converted to NULL without no error.
  static vtkObjectBase *GetPointerFromObject(PyObject *obj,
                                             const char *classname);

  // Description:
  // Convert a vtkObjectBase to a PyVTKObject.  This will first check to
  // see if the PyVTKObject already exists, and create a new PyVTKObject
  // if necessary.  This function also passes ownership of the reference
  // to the PyObject.
  // Special behaviour: NULL is converted to Py_None.
  static PyObject *GetObjectFromPointer(vtkObjectBase *ptr);

  // Description:
  // Extract the SIP wrapped object from a PyObject.  If the conversion cannot
  // be done, an error indicator is set.
  // Special behavior: Py_None is converted to NULL without no error.
  static void *SIPGetPointerFromObject(PyObject *obj, const char *classname);

  // Description:
  // Convert a SIP wrapped object to a PyObject.
  // Special behaviour: NULL is converted to Py_None.
  static PyObject *SIPGetObjectFromPointer(
    const void *ptr, const char* classname, bool is_new);

  // Description:
  // Try to convert some PyObject into a PyVTKObject, currently conversion
  // is supported for SWIG-style mangled pointer strings.
  static PyObject *GetObjectFromObject(PyObject *arg, const char *type);

  // Description:
  // Add PyVTKObject/vtkObjectBase pairs to the internal mapping.
  // This methods do not change the reference counts of either the
  // vtkObjectBase or the PyVTKObject.
  static void AddObjectToMap(PyObject *obj, vtkObjectBase *anInstance);

  // Description:
  // Remove a PyVTKObject from the internal mapping.  No reference
  // counts are changed.
  static void RemoveObjectFromMap(PyObject *obj);

  // Description:
  // Add a special VTK type to the type lookup table, this allows us to
  // later create object given only the class name.
  static PyVTKSpecialType *AddSpecialTypeToMap(
    PyTypeObject *pytype, PyMethodDef *methods, PyMethodDef *constructors,
    const char *docstring[], PyVTKSpecialCopyFunc copyfunc);

  // Description:
  // Get information about a special VTK type, given the type name.
  static PyVTKSpecialType *FindSpecialType(const char *classname);

  // Description:
  // Given a PyObject, convert it into a "result_type" object, where
  // "result_type" must be a wrapped type.  The C object is returned
  // as a void *, which must be cast to a pointer of the desired type.
  // If conversion was necessary, then the created python object is
  // returned in "newobj", but if the original python object was
  // already of the correct type, then "newobj" will be set to NULL.
  // If a python exception was raised, NULL will be returned.
  static void *GetPointerFromSpecialObject(
    PyObject *obj, const char *result_type, PyObject **newobj);

  // Description:
  // Add a wrapped C++ namespace as a python module object.  This allows
  // the namespace to be retrieved and added to as necessary.
  static void AddNamespaceToMap(PyObject *o);

  // Description:
  // Remove a wrapped C++ namespace from consideration.  This is called
  // from the namespace destructor.
  static void RemoveNamespaceFromMap(PyObject *o);

  // Description:
  // Return an existing namespace, or NULL if it doesn't exist.
  static PyObject *FindNamespace(const char *name);

  // Description:
  // Utility function to build a docstring by concatenating a series
  // of strings until a null string is found.
  static PyObject *BuildDocString(const char *docstring[]);

  // Description:
  // Utility function for creating SWIG-style mangled pointer string.
  static char *ManglePointer(const void *ptr, const char *type);

  // Description:
  // Utility function decoding a SWIG-style mangled pointer string.
  static void *UnmanglePointer(char *ptrText, int *len, const char *type);

  // Description:
  // Compute a hash for a vtkVariant.
  static long VariantHash(const vtkVariant *variant);

  // Description:
  // Register a vtkPythonCommand. Registering vtkPythonCommand instances ensures
  // that when the interpreter is destroyed (and Py_AtExit() gets called), the
  // vtkPythonCommand state is updated to avoid referring to dangling Python
  // objects pointers. Note, this will not work with Py_NewInterpreter.
  static void RegisterPythonCommand(vtkPythonCommand*);
  static void UnRegisterPythonCommand(vtkPythonCommand*);

private:
  vtkPythonUtil();
  ~vtkPythonUtil();
  vtkPythonUtil(const vtkPythonUtil&);  // Not implemented.
  void operator=(const vtkPythonUtil&);  // Not implemented.

  vtkPythonObjectMap *ObjectMap;
  vtkPythonGhostMap *GhostMap;
  vtkPythonClassMap *ClassMap;
  vtkPythonSpecialTypeMap *SpecialTypeMap;
  vtkPythonNamespaceMap *NamespaceMap;
  vtkPythonCommandList *PythonCommandList;

  friend void vtkPythonUtilDelete();
  friend void vtkPythonUtilCreateIfNeeded();
};

// For use by SetXXMethod() , SetXXMethodArgDelete()
extern VTKWRAPPINGPYTHONCORE_EXPORT void vtkPythonVoidFunc(void *);
extern VTKWRAPPINGPYTHONCORE_EXPORT void vtkPythonVoidFuncArgDelete(void *);

// The following macro is used to suppress missing initializer
// warnings.  Python documentation says these should not be necessary.
// We define it as a macro in case the length needs to change across
// python versions.
#if   PY_VERSION_HEX >= 0x02060000 // for tp_version_tag
#define VTK_PYTHON_UTIL_SUPRESS_UNINITIALIZED \
  0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0, 0,
#define VTK_WRAP_PYTHON_SUPRESS_UNINITIALIZED \
  0, 0,
#endif

#endif
