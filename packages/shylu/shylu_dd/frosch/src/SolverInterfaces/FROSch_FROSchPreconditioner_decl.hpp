//@HEADER
// ************************************************************************
//
//               ShyLU: Hybrid preconditioner package
//                 Copyright 2012 Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Alexander Heinlein (alexander.heinlein@uni-koeln.de)
//
// ************************************************************************
//@HEADER

#ifndef _FROSCH_FROSCHPRECONDITIONER_DECL_HPP
#define _FROSCH_FROSCHPRECONDITIONER_DECL_HPP

#include <ShyLU_DDFROSch_config.h>

// FROSch
#include <FROSch_Solver_def.hpp>

#include <FROSch_SchwarzPreconditioners_fwd.hpp>


namespace FROSch {

    using namespace Teuchos;
    using namespace Xpetra;

    template <class SC = double,
              class LO = int,
              class GO = DefaultGlobalOrdinal,
              class NO = Tpetra::KokkosClassic::DefaultNode::DefaultNodeType>
    class FROSchPreconditioner : public Solver<SC,LO,GO,NO> {

    protected:

        // Xpetra
        using ConstXMatrixPtr                   = typename Solver<SC,LO,GO,NO>::ConstXMatrixPtr;

        using XMultiVector                      = MultiVector<SC,LO,GO,NO>;
        using ConstXMultiVector                 = const MultiVector<SC,LO,GO,NO>;
        using XMultiVectorPtr                   = RCP<XMultiVector>;
        using ConstXMultiVectorPtr              = RCP<const XMultiVector>;
        using ConstXMultiVectorPtrVecPtr        = ArrayRCP<ConstXMultiVectorPtr>;

        using XMultiVectorFactory               = typename Solver<SC,LO,GO,NO>::XMultiVectorFactory;

        // Teuchos
        using ParameterListPtr                  = typename Solver<SC,LO,GO,NO>::ParameterListPtr;

        // FROSch
        using SchwarzPreconditionerPtr          = RCP<SchwarzPreconditioner<SC,LO,GO,NO> >;
        using TwoLevelBlockPreconditionerPtr    = RCP<TwoLevelBlockPreconditioner<SC,LO,GO,NO> >;
        using TwoLevelPreconditionerPtr         = RCP<TwoLevelPreconditioner<SC,LO,GO,NO> >;

        using UN                                = unsigned;
        using UNVec                             = Array<UN>;
        using UNVecPtr                          = ArrayRCP<UN>;

        using GOVecPtr                          = ArrayRCP<GO>;

    public:

        //! Initialize the internal solver
        virtual int initialize();

        //! Compute the internal solver
        virtual int compute();

        /*!
        \brief Computes the operator-multivector application.

        Loosely, performs \f$Y = \alpha \cdot A^{\textrm{mode}} \cdot X + \beta \cdot Y\f$. However, the details of operation
        vary according to the values of \c alpha and \c beta. Specifically
        - if <tt>beta == 0</tt>, apply() <b>must</b> overwrite \c Y, so that any values in \c Y (including NaNs) are ignored.
        - if <tt>alpha == 0</tt>, apply() <b>may</b> short-circuit the operator, so that any values in \c X (including NaNs) are ignored.
        */
        void apply(const XMultiVector &x,
                   XMultiVector &y,
                   ETransp mode=NO_TRANS,
                   SC alpha=ScalarTraits<SC>::one(),
                   SC beta=ScalarTraits<SC>::zero()) const;

        int updateMatrix(ConstXMatrixPtr k,
                         bool reuseInitialize=false);

    protected:

        //! Constructor
        FROSchPreconditioner(ConstXMatrixPtr k,
                             ParameterListPtr parameterList,
                             string description);

        SchwarzPreconditionerPtr FROSchPreconditioner_ = null;

        friend class SolverFactory<SC,LO,GO,NO>;
    };

}

#endif
