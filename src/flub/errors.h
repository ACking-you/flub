#pragma once
#include <string>

#include "option.h"
#include "result.h"
#include "triats.h"
#include "types.h"

namespace flub {
/// Base error type
template <typename E, typename V>
struct Error
{
   E code;
   V detail;
   Error(E c, V &&d) : code(c), detail(std::forward<V>(d))
   {
      static_assert(
        std::is_convertible<E, int>::value || std::is_enum<E>::value,
        "`code` must be convertible to int");
   }
};

template <typename E>
struct Error<E, StrRef>
{
   E      code;
   StrRef detail;
   Error(E c, const StrRef &v) : code(c), detail(v)
   {
      static_assert(
        std::is_convertible<E, int>::value || std::is_enum<E>::value,
        "`code` must be convertible to int");
   }
};

template <typename E>
struct Error<E, std::string>
{
   E           code;
   std::string detail;
   Error(E c, std::string v) : code(c), detail(std::move(v))
   {
      static_assert(
        std::is_convertible<E, int>::value || std::is_enum<E>::value,
        "`code` must be convertible to int");
   }
};

// Boxed error type for generic type
class BoxedError
{
public:
   template <typename E, typename V>
   BoxedError(Error<E, V> &&error)
   {
      static_assert(has_to_string<V>::value,
                    "Not supported type,please implement `to_string` method "
                    "for this type!");

      type_hash_     = typeid(E).hash_code();
      error_code_    = static_cast<int>(error.code);
      detail_string_ = error.detail.to_string();
   }
   template <class E>
   BoxedError(Error<E, std::string> &&error)
     : type_hash_(typeid(E).hash_code()),
       error_code_(static_cast<int>(error.code)),
       detail_string_(error.detail)
   {
   }

   template <class E>
   BoxedError(Error<E, StrRef> &&error)
     : type_hash_(typeid(E).hash_code()),
       error_code_(static_cast<int>(error.code)),
       detail_string_(error.detail.data(), error.detail.size())
   {
   }

   template <typename E, typename V>
   static BoxedError from(Error<E, V> error)
   {
      return BoxedError{std::forward<Error<E, V>>(error)};
   }

   template <typename E>
   bool is_type() const
   {
      static_assert(
        std::is_convertible<E, int>::value || std::is_enum<E>::value,
        "`code` must be convertible to int");
      return type_hash_ == typeid(E).hash_code();
   }

   template <typename E>
   Option<E> to_enum()
   {
      static_assert(
        std::is_convertible<E, int>::value || std::is_enum<E>::value,
        "`code` must be convertible to int");
      if (is_type<E>()) { return static_cast<E>(error_code_); }
      return {};
   }

   int error_code() const { return error_code_; }

   std::string       &string() { return detail_string_; }
   const std::string &string() const { return detail_string_; }

private:
   size_t      type_hash_;
   int         error_code_;
   std::string detail_string_;
};

template <typename E, typename V>
Err<Error<E, V>> make_error(E c, V &&v)
{
   return Err<Error<E, V>>(Error<E, V>{c, std::forward<V>(v)});
}

/// Transforming Error Type into BoxedError
template <typename E, typename V>
Err<BoxedError> operator--(Err<Error<E, V>> &err)
{
   return Err<BoxedError>{std::move(err.value())};
}

/// Transforming BoxedError Type into Error<E,string>
template <typename E>
Option<Err<Error<E, std::string>>> to_error(Err<BoxedError> &item)
{
   auto &err = item.value();
   auto  e   = err.to_enum<E>();
   if (!e) { return {}; }
   auto ee = Error<E, std::string>{*e, std::move(err.string())};
   return Err<Error<E, std::string>>(std::move(ee));
}

}   // namespace flub