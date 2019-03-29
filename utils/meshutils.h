#ifndef MESHUTILS_H
#define MESHUTILS_H

#include <string>

#include <vcg/complex/algorithms/clean.h>
#include <vcg/complex/algorithms/update/topology.h>

struct MeshUtils {

    /*! \brief Delete (if exists) and create new attributes
     * \param mesh mesh that holds the attribute
     * \param attr attribute name
     * \return The attribute container
     */
    template <typename MeshType, typename AttrType>
    static typename MeshType::template PerFaceAttributeHandle<AttrType>
    resetFaceAttr(MeshType &mesh, const std::string &attr) {
        bool hasPatchId = vcg::tri::HasPerFaceAttribute(mesh, attr);
        if (hasPatchId) {
            vcg::tri::Allocator<MeshType>::DeletePerFaceAttribute(mesh, attr);
        }

        return vcg::tri::Allocator<MeshType>::template GetPerFaceAttribute<
            AttrType>(mesh, attr);
    }
};

#endif // MESHUTILS_H
