// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef KERNELKIT_USERMGR_USER_H
#define KERNELKIT_USERMGR_USER_H

/* ========================================

 Revision History:

 04/03/25: Set users directory as /users/ instead of /usr/

 ======================================== */

#include <CompilerKit/CompilerKit.h>
#include <KernelKit/KPC.h>
#include <NeKit/Config.h>
#include <NeKit/KString.h>

///! We got the MGMT, STD (%s format) and GUEST users,
///! all are used to make authorized operations.
#define kMgmtUser "NE.APP/MGMT/%s"
#define kGuestUser "NE.APP/GUEST/%s"
#define kStdUser "NE.APP/STD/%s"

///! Built-in users, bound by user_init_std.
#define kRootUserName "NE.APP/MGMT/root"
#define kGuestUserName "NE.APP/GUEST/guest"
#define kRecoveryUserName "NE.APP/GUEST/recov"

#define kUsersDir "/users/"

#define kMaxUserNameLen (256U)
#define kMaxUserTokenLen (256U)

namespace Ne::Kernel {

class User;

enum struct UserRingKind : Int32 {
  kRingInvalid   = 0,
  kRingStdUser   = 444,
  kRingSuperUser = 666,
  kRingGuestUser = 777,
  kRingRecoveryUser = 888,
  kRingCount     = 3,
};

using UserPublicKey     = Char8*;
using UserPublicKeyType = Char8;

/// @brief System User class.
class User final {
 public:
  User() = delete;

  User(const Int32& sel, const Char* username);
  User(const UserRingKind& kind, const Char* username);

  ~User();

 public:
  NE_COPY_DEFAULT(User)

 public:
  bool operator==(const User& lhs);
  bool operator!=(const User& lhs);

 public:
  /// @brief Get software ring
  const UserRingKind& Ring();

  /// @brief Get user name
  Char* Name();

  /// @brief Is he a standard user?
  Bool IsStdUser();

  /// @brief Is she a super user?
  Bool IsSuperUser();

  /// @brief Is he a guest user?
  Bool IsGuestUser();

  /// @brief Saves a password from the public key.
  Bool Save(const UserPublicKey password);

  /// @brief Checks if a password matches the **password**.
  /// @param password the password to check.
  Bool Login(const UserPublicKey password);

  /// @brief Returns whether the user is an adult or not. 
  Bool IsAdult();

 private:
  UserRingKind mUserRing{UserRingKind::kRingStdUser};
  Char         mUserName[kMaxUserNameLen] = {0};
  UInt64       mUserFNV{0UL};
  Bool         mUserIsAdult{NO};

};

/// \brief Alias for user ptr.
using UserPtr = User*;

/// \brief Current user pointer.
inline UserPtr kCurrentUser = nullptr;

/// \brief Supervisor pointer.
inline UserPtr kRootUser = nullptr;

/// \brief Guest user pointer.
inline UserPtr kGuest = nullptr;

/// \brief RPC/BaseHost user pointer.
inline UserPtr kHostUser = nullptr;

/// @brief Bind the built-in users.
/// @param recovery make a recovery user when asked to.
Void user_init_std(const Bool recovery);

}  // namespace Ne::Kernel

#endif /* ifndef KERNELKIT_USERMGR_USER_H */
