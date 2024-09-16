/*
//@HEADER
// ************************************************************************
//
//                        Kokkos v. 2.0
//              Copyright (2019) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software. //
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
// Questions? Contact Christian R. Trott (crtrott@sandia.gov)
//
// ************************************************************************
//@HEADER
*/

#ifndef LINALG_INCLUDE_EXPERIMENTAL___P1673_BITS_BLAS3_MATRIX_RANK_K_UPDATE_HPP_
#define LINALG_INCLUDE_EXPERIMENTAL___P1673_BITS_BLAS3_MATRIX_RANK_K_UPDATE_HPP_

namespace MDSPAN_IMPL_STANDARD_NAMESPACE {
namespace MDSPAN_IMPL_PROPOSED_NAMESPACE {
inline namespace __p1673_version_0 {
namespace linalg {

namespace
{

template <class Exec, class ScaleFactorType, class A_t, class C_t, class Tr_t, class = void>
struct is_custom_sym_mat_rank_k_update_avail : std::false_type {};

template <class Exec, class A_t, class C_t, class Tr_t>
struct is_custom_sym_mat_rank_k_update_avail<
  Exec, void, A_t, C_t, Tr_t,
  std::enable_if_t<
    std::is_void_v<
      decltype(symmetric_matrix_rank_k_update(std::declval<Exec>(),
					      std::declval<A_t>(),
					      std::declval<C_t>(),
					      std::declval<Tr_t>()))
      >
    && ! impl::is_inline_exec_v<Exec>
    >
  >
  : std::true_type{};

template <class Exec, class ScaleFactorType, class A_t, class C_t, class Tr_t>
struct is_custom_sym_mat_rank_k_update_avail<
  Exec, ScaleFactorType, A_t, C_t, Tr_t,
  std::enable_if_t<
    std::is_void_v<
      decltype(symmetric_matrix_rank_k_update(std::declval<Exec>(),
					      std::declval<ScaleFactorType>(),
					      std::declval<A_t>(),
					      std::declval<C_t>(),
					      std::declval<Tr_t>()))
      >
    && ! impl::is_inline_exec_v<Exec>
    >
  >
  : std::true_type{};


template <class Exec, class ScaleFactorType, class A_t, class C_t, class Tr_t, class = void>
struct is_custom_herm_mat_rank_k_update_avail : std::false_type {};

template <class Exec, class A_t, class C_t, class Tr_t>
struct is_custom_herm_mat_rank_k_update_avail<
  Exec, void, A_t, C_t, Tr_t,
  std::enable_if_t<
    std::is_void_v<
      decltype(hermitian_matrix_rank_k_update(std::declval<Exec>(),
					      std::declval<A_t>(),
					      std::declval<C_t>(),
					      std::declval<Tr_t>()))
      >
    && ! impl::is_inline_exec_v<Exec>
    >
  >
  : std::true_type{};

template <class Exec, class ScaleFactorType, class A_t, class C_t, class Tr_t>
struct is_custom_herm_mat_rank_k_update_avail<
  Exec, ScaleFactorType, A_t, C_t, Tr_t,
  std::enable_if_t<
    std::is_void_v<
      decltype(hermitian_matrix_rank_k_update(std::declval<Exec>(),
					      std::declval<ScaleFactorType>(),
					      std::declval<A_t>(),
					      std::declval<C_t>(),
					      std::declval<Tr_t>()))
      >
    && ! impl::is_inline_exec_v<Exec>
    >
  >
  : std::true_type{};

} //end anonym namespace

// Rank-k update of a symmetric matrix with scaling factor alpha

MDSPAN_TEMPLATE_REQUIRES(
  class ScaleFactorType,
  class ElementType_A,
  class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
  class Layout_A,
  class Accessor_A,
  class ElementType_C,
  class SizeType_C, ::std::size_t numRows_C, ::std::size_t numCols_C,
  class Layout_C,
  class Accessor_C,
  class Triangle,
  /* requires */ (
    std::is_same_v<Triangle, lower_triangle_t> ||
    std::is_same_v<Triangle, upper_triangle_t>
  )
)
void symmetric_matrix_rank_k_update(
  impl::inline_exec_t&& /* exec */,
  ScaleFactorType alpha,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  mdspan<ElementType_C, extents<SizeType_C, numRows_C, numCols_C>, Layout_C, Accessor_C> C,
  Triangle /* t */)
{
  constexpr bool lower_tri =
    std::is_same_v<Triangle, lower_triangle_t>;
  using size_type = std::common_type_t<SizeType_A, SizeType_C>;

  for (size_type j = 0; j < C.extent(1); ++j) {
    const size_type i_lower = lower_tri ? j : size_type(0);
    const size_type i_upper = lower_tri ? C.extent(0) : j+1;
    for (size_type i = i_lower; i < i_upper; ++i) {
      for (size_type k = 0; k < A.extent(1); ++k) {
          C(i, j) += alpha * A(i, k) * A(j, k);
      }
    }
  }
}

MDSPAN_TEMPLATE_REQUIRES(
  class ExecutionPolicy,
  class ScaleFactorType,
  class ElementType_A,
  class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
  class Layout_A,
  class Accessor_A,
  class ElementType_C,
  class SizeType_C, ::std::size_t numRows_C, ::std::size_t numCols_C,
  class Layout_C,
  class Accessor_C,
  class Triangle,
  /* requires */ (
    impl::is_linalg_execution_policy_other_than_inline_v<ExecutionPolicy> &&
    (std::is_same_v<Triangle, lower_triangle_t> ||
     std::is_same_v<Triangle, upper_triangle_t>)
  )
)
void symmetric_matrix_rank_k_update(
  ExecutionPolicy&& exec,
  ScaleFactorType alpha,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  mdspan<ElementType_C, extents<SizeType_C, numRows_C, numCols_C>, Layout_C, Accessor_C> C,
  Triangle t)
{
  constexpr bool use_custom = is_custom_sym_mat_rank_k_update_avail<
    decltype(impl::map_execpolicy_with_check(exec)),
    ScaleFactorType, decltype(A), decltype(C), Triangle>::value;

  if constexpr (use_custom) {
    symmetric_matrix_rank_k_update(impl::map_execpolicy_with_check(exec), alpha, A, C, t);
  } else {
    symmetric_matrix_rank_k_update(impl::inline_exec_t{}, alpha, A, C, t);
  }
}

MDSPAN_TEMPLATE_REQUIRES(
  class ScaleFactorType,
  class ElementType_A,
  class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
  class Layout_A,
  class Accessor_A,
  class ElementType_C,
  class SizeType_C, ::std::size_t numRows_C, ::std::size_t numCols_C,
  class Layout_C,
  class Accessor_C,
  class Triangle,
  /* requires */ (
    (! impl::is_linalg_execution_policy_other_than_inline_v<ScaleFactorType>) &&
    (std::is_same_v<Triangle, lower_triangle_t> ||
     std::is_same_v<Triangle, upper_triangle_t>)
  )
)
void symmetric_matrix_rank_k_update(
  ScaleFactorType alpha,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  mdspan<ElementType_C, extents<SizeType_C, numRows_C, numCols_C>, Layout_C, Accessor_C> C,
  Triangle t)
{
  symmetric_matrix_rank_k_update(impl::default_exec_t{}, alpha, A, C, t);
}

// Rank-k update of a symmetric matrix without scaling factor alpha

MDSPAN_TEMPLATE_REQUIRES(
  class ElementType_A,
  class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
  class Layout_A,
  class Accessor_A,
  class ElementType_C,
  class SizeType_C, ::std::size_t numRows_C, ::std::size_t numCols_C,
  class Layout_C,
  class Accessor_C,
  class Triangle,
  /* requires */ (
    std::is_same_v<Triangle, lower_triangle_t> ||
    std::is_same_v<Triangle, upper_triangle_t>
  )
)
void symmetric_matrix_rank_k_update(
  impl::inline_exec_t&& /* exec */,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  mdspan<ElementType_C, extents<SizeType_C, numRows_C, numCols_C>, Layout_C, Accessor_C> C,
  Triangle /* t */)
{
  constexpr bool lower_tri =
    std::is_same_v<Triangle, lower_triangle_t>;
  using size_type = std::common_type_t<SizeType_A, SizeType_C>;

  for (size_type j = 0; j < C.extent(1); ++j) {
    const size_type i_lower = lower_tri ? j : size_type(0);
    const size_type i_upper = lower_tri ? C.extent(0) : j+1;
    for (size_type i = i_lower; i < i_upper; ++i) {
      for (size_type k = 0; k < A.extent(1); ++k) {
          C(i, j) += A(i, k) * A(j, k);
      }
    }
  }
}

MDSPAN_TEMPLATE_REQUIRES(
  class ExecutionPolicy,
  class ElementType_A,
  class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
  class Layout_A,
  class Accessor_A,
  class ElementType_C,
  class SizeType_C, ::std::size_t numRows_C, ::std::size_t numCols_C,
  class Layout_C,
  class Accessor_C,
  class Triangle,
  /* requires */ (
    impl::is_linalg_execution_policy_other_than_inline_v<ExecutionPolicy> &&
    (std::is_same_v<Triangle, lower_triangle_t> ||
     std::is_same_v<Triangle, upper_triangle_t>)
  )
)
void symmetric_matrix_rank_k_update(
  ExecutionPolicy&& exec,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  mdspan<ElementType_C, extents<SizeType_C, numRows_C, numCols_C>, Layout_C, Accessor_C> C,
  Triangle t)
{
  constexpr bool use_custom = is_custom_sym_mat_rank_k_update_avail<
    decltype(impl::map_execpolicy_with_check(exec)), void, decltype(A), decltype(C), Triangle
    >::value;

  if constexpr (use_custom) {
    symmetric_matrix_rank_k_update(impl::map_execpolicy_with_check(exec), A, C, t);
  } else {
    symmetric_matrix_rank_k_update(impl::inline_exec_t{}, A, C, t);
  }
}

MDSPAN_TEMPLATE_REQUIRES(
  class ElementType_A,
  class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
  class Layout_A,
  class Accessor_A,
  class ElementType_C,
  class SizeType_C, ::std::size_t numRows_C, ::std::size_t numCols_C,
  class Layout_C,
  class Accessor_C,
  class Triangle,
  /* requires */ (
    std::is_same_v<Triangle, lower_triangle_t> ||
    std::is_same_v<Triangle, upper_triangle_t>
  )
)
void symmetric_matrix_rank_k_update(
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  mdspan<ElementType_C, extents<SizeType_C, numRows_C, numCols_C>, Layout_C, Accessor_C> C,
  Triangle t)
{
  symmetric_matrix_rank_k_update(impl::default_exec_t{}, A, C, t);
}

// Rank-k update of a Hermitian matrix

// Rank-k update of a Hermitian matrix with scaling factor alpha

MDSPAN_TEMPLATE_REQUIRES(
  class ScaleFactorType,
  class ElementType_A,
  class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
  class Layout_A,
  class Accessor_A,
  class ElementType_C,
  class SizeType_C, ::std::size_t numRows_C, ::std::size_t numCols_C,
  class Layout_C,
  class Accessor_C,
  class Triangle,
  /* requires */ (
    std::is_same_v<Triangle, lower_triangle_t> ||
    std::is_same_v<Triangle, upper_triangle_t>
  )
)
void hermitian_matrix_rank_k_update(
  impl::inline_exec_t&& /* exec */,
  ScaleFactorType alpha,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  mdspan<ElementType_C, extents<SizeType_C, numRows_C, numCols_C>, Layout_C, Accessor_C> C,
  Triangle /* t */)
{
  using size_type = std::common_type_t<SizeType_A, SizeType_C>;

  constexpr bool lower_tri =
    std::is_same_v<Triangle, lower_triangle_t>;
  using size_type = std::common_type_t<SizeType_A, SizeType_C>;

  for (size_type j = 0; j < C.extent(1); ++j) {
    const size_type i_lower = lower_tri ? j : size_type(0);
    const size_type i_upper = lower_tri ? C.extent(0) : j+1;
    for (size_type i = i_lower; i < i_upper; ++i) {
      for (size_type k = 0; k < A.extent(1); ++k) {
          C(i, j) += alpha * A(i, k) * impl::conj_if_needed(A(j, k));
      }
    }
  }
}

MDSPAN_TEMPLATE_REQUIRES(
  class ExecutionPolicy,
  class ScaleFactorType,
  class ElementType_A,
  class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
  class Layout_A,
  class Accessor_A,
  class ElementType_C,
  class SizeType_C, ::std::size_t numRows_C, ::std::size_t numCols_C,
  class Layout_C,
  class Accessor_C,
  class Triangle,
  /* requires */ (
    impl::is_linalg_execution_policy_other_than_inline_v<ExecutionPolicy> &&
    (std::is_same_v<Triangle, lower_triangle_t> ||
     std::is_same_v<Triangle, upper_triangle_t>)
  )
)
void hermitian_matrix_rank_k_update(
  ExecutionPolicy&& exec,
  ScaleFactorType alpha,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  mdspan<ElementType_C, extents<SizeType_C, numRows_C, numCols_C>, Layout_C, Accessor_C> C,
  Triangle t)
{
  constexpr bool use_custom = is_custom_herm_mat_rank_k_update_avail<
    decltype(impl::map_execpolicy_with_check(exec)),
    ScaleFactorType, decltype(A), decltype(C), Triangle>::value;

  if constexpr (use_custom) {
    hermitian_matrix_rank_k_update(impl::map_execpolicy_with_check(exec), alpha, A, C, t);
  } else {
    hermitian_matrix_rank_k_update(impl::inline_exec_t{}, alpha, A, C, t);
  }
}

MDSPAN_TEMPLATE_REQUIRES(
  class ScaleFactorType,
  class ElementType_A,
  class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
  class Layout_A,
  class Accessor_A,
  class ElementType_C,
  class SizeType_C, ::std::size_t numRows_C, ::std::size_t numCols_C,
  class Layout_C,
  class Accessor_C,
  class Triangle,
  /* requires */ (
    (! impl::is_linalg_execution_policy_other_than_inline_v<ScaleFactorType>) &&
    (std::is_same_v<Triangle, lower_triangle_t> ||
     std::is_same_v<Triangle, upper_triangle_t>)
  )
)
void hermitian_matrix_rank_k_update(
  ScaleFactorType alpha,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  mdspan<ElementType_C, extents<SizeType_C, numRows_C, numCols_C>, Layout_C, Accessor_C> C,
  Triangle t)
{
  hermitian_matrix_rank_k_update(impl::default_exec_t{}, alpha, A, C, t);
}

// Rank-k update of a Hermitian matrix without scaling factor alpha

MDSPAN_TEMPLATE_REQUIRES(
  class ElementType_A,
  class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
  class Layout_A,
  class Accessor_A,
  class ElementType_C,
  class SizeType_C, ::std::size_t numRows_C, ::std::size_t numCols_C,
  class Layout_C,
  class Accessor_C,
  class Triangle,
  /* requires */ (
    std::is_same_v<Triangle, lower_triangle_t> ||
    std::is_same_v<Triangle, upper_triangle_t>
  )
)
void hermitian_matrix_rank_k_update(
  impl::inline_exec_t&& /* exec */,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  mdspan<ElementType_C, extents<SizeType_C, numRows_C, numCols_C>, Layout_C, Accessor_C> C,
  Triangle /* t */)
{
  using size_type = std::common_type_t<SizeType_A, SizeType_C>;

  constexpr bool lower_tri =
    std::is_same_v<Triangle, lower_triangle_t>;
  using size_type = std::common_type_t<SizeType_A, SizeType_C>;

  for (size_type j = 0; j < C.extent(1); ++j) {
    const size_type i_lower = lower_tri ? j : size_type(0);
    const size_type i_upper = lower_tri ? C.extent(0) : j+1;
    for (size_type i = i_lower; i < i_upper; ++i) {
      for (size_type k = 0; k < A.extent(1); ++k) {
          C(i, j) += A(i, k) * impl::conj_if_needed(A(j, k));
      }
    }
  }
}

MDSPAN_TEMPLATE_REQUIRES(
  class ExecutionPolicy,
  class ElementType_A,
  class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
  class Layout_A,
  class Accessor_A,
  class ElementType_C,
  class SizeType_C, ::std::size_t numRows_C, ::std::size_t numCols_C,
  class Layout_C,
  class Accessor_C,
  class Triangle,
  /* requires */ (
    impl::is_linalg_execution_policy_other_than_inline_v<ExecutionPolicy> &&
    (std::is_same_v<Triangle, lower_triangle_t> ||
     std::is_same_v<Triangle, upper_triangle_t>)
  )
)
void hermitian_matrix_rank_k_update(
  ExecutionPolicy&& exec,
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  mdspan<ElementType_C, extents<SizeType_C, numRows_C, numCols_C>, Layout_C, Accessor_C> C,
  Triangle t)
{
  constexpr bool use_custom = is_custom_herm_mat_rank_k_update_avail<
    decltype(impl::map_execpolicy_with_check(exec)),
    void, decltype(A), decltype(C), Triangle>::value;

  if constexpr (use_custom) {
    hermitian_matrix_rank_k_update(impl::map_execpolicy_with_check(exec), A, C, t);
  } else {
    hermitian_matrix_rank_k_update(impl::inline_exec_t{}, A, C, t);
  }
}

MDSPAN_TEMPLATE_REQUIRES(
  class ElementType_A,
  class SizeType_A, ::std::size_t numRows_A, ::std::size_t numCols_A,
  class Layout_A,
  class Accessor_A,
  class ElementType_C,
  class SizeType_C, ::std::size_t numRows_C, ::std::size_t numCols_C,
  class Layout_C,
  class Accessor_C,
  class Triangle,
  /* requires */ (
    std::is_same_v<Triangle, lower_triangle_t> ||
    std::is_same_v<Triangle, upper_triangle_t>
  )
)
void hermitian_matrix_rank_k_update(
  mdspan<ElementType_A, extents<SizeType_A, numRows_A, numCols_A>, Layout_A, Accessor_A> A,
  mdspan<ElementType_C, extents<SizeType_C, numRows_C, numCols_C>, Layout_C, Accessor_C> C,
  Triangle t)
{
  hermitian_matrix_rank_k_update(impl::default_exec_t{}, A, C, t);
}

} // end namespace linalg
} // end inline namespace __p1673_version_0
} // end namespace MDSPAN_IMPL_PROPOSED_NAMESPACE
} // end namespace MDSPAN_IMPL_STANDARD_NAMESPACE

#endif //LINALG_INCLUDE_EXPERIMENTAL___P1673_BITS_BLAS3_MATRIX_RANK_K_UPDATE_HPP_
