#pragma once

#include <cstddef>
#include <type_traits>

namespace typeSet {

  template <typename...>
  struct Set {
  };

  template <typename... E>
  constexpr size_t size(Set<E...>)
  {
    return sizeof...(E);
  }

  template <typename A>
  constexpr size_t size()
  {
    return size(A{});
  }

  template <typename>
  constexpr bool contain(Set<>)
  {
    return false;
  }

  template <typename A, typename B, typename... E>
  constexpr bool contain(Set<B, E...>)
  {
    if constexpr (std::is_same_v<A, B>) return true;
    return contain<A>(Set<E...>{});
  }

  template <typename A, typename S>
  constexpr bool contain()
  {
    return contain<A>(S{});
  }

  template <typename>
  constexpr bool subset(Set<>)
  {
    return true;
  }

  template <typename B, typename AH, typename... AT>
  constexpr bool subset(Set<AH, AT...>)
  {
    return contain<AH, B>() && subset<B>(Set<AT...>{});
  }

  template <typename A, typename B>
  constexpr bool subset()
  {
    return subset<B>(A{});
  }

  template <typename A, typename B>
  constexpr bool equal()
  {
    return subset<A, B>() && subset<B, A>();
  }

  template <typename, typename>
  struct InsertT {
  };

  template <typename A, typename... B>
  struct InsertT<A, Set<B...>> {
    using type = std::conditional_t<contain<A, Set<B...>>(),
                                    Set<B...>,
                                    Set<B..., A>>;
  };

  template <typename... A>
  using Insert = typename InsertT<A...>::type;

  template <typename A, typename>
  struct UnionT {
    using type = A;
  };

  template <typename A, typename B, typename... C>
  struct UnionT<A, Set<B, C...>> {
    using type = typename UnionT<Insert<B, A>, Set<C...>>::type;
  };

  template <typename... A>
  using Union = typename UnionT<A...>::type;

  template <typename, typename>
  struct RemoveT {
    using type = Set<>;
  };

  template <typename A, typename B, typename... C>
  struct RemoveT<A, Set<B, C...>> {
    using type = std::conditional_t<
        std::is_same_v<A, B>,
        Set<C...>,
        Union<Set<B>, typename RemoveT<A, Set<C...>>::type>>;
  };

  template <typename... A>
  using Remove = typename RemoveT<A...>::type;

  template <typename A, typename>
  struct IntersectT {
    using type = Set<>;
  };

  template <typename A, typename B, typename... C>
  struct IntersectT<A, Set<B, C...>> {
    using type = typename std::conditional_t<
        contain<B, A>(),
        Union<Set<B>, typename IntersectT<A, Set<C...>>::type>,
        typename IntersectT<A, Set<C...>>::type>;
  };

  template <typename... A>
  using Intersect = typename IntersectT<A...>::type;

  template <typename...>
  struct DifferenceT {
  };

  template <typename... A>
  struct DifferenceT<Set<A...>, Set<>> {
    using type = Set<A...>;
  };

  template <typename A, typename B, typename... C>
  struct DifferenceT<A, Set<B, C...>> {
    using type = typename DifferenceT<Remove<B, A>, Set<C...>>::type;
  };

  template <typename... A>
  using Difference = typename DifferenceT<A...>::type;

}  // namespace typeSet
