// Copyright 2014 Toggl Desktop developers.

#ifndef SRC_BASE_MODEL_H_
#define SRC_BASE_MODEL_H_

#include <string>
#include <vector>

#include "./types.h"

#include "Poco/Types.h"

namespace kopsik {

  class BaseModel {
  public:
    BaseModel()
      : local_id_(0)
      , id_(0)
      , guid_("")
      , ui_modified_at_(0)
      , uid_(0)
      , dirty_(false)
      , deleted_at_(0)
      , is_marked_as_deleted_on_server_(false)
      , updated_at_(0) {}
    virtual ~BaseModel() {}

    Poco::Int64 LocalID() const { return local_id_; }
    void SetLocalID(const Poco::Int64 value) { local_id_ = value; }

    Poco::UInt64 ID() const { return id_; }
    void SetID(const Poco::UInt64 value);

    Poco::UInt64 UIModifiedAt() const { return ui_modified_at_; }
    void SetUIModifiedAt(const Poco::UInt64 value);

    std::string GUID() const { return guid_; }
    void SetGUID(const std::string value);

    Poco::UInt64 UID() const { return uid_; }
    void SetUID(const Poco::UInt64 value);

    void SetDirty() { dirty_ = true; }
    bool Dirty() const { return dirty_; }
    void ClearDirty() { dirty_ = false; }

    // Deleting a time entry hides it from
    // UI and flags it for removal from server:
    Poco::UInt64 DeletedAt() const { return deleted_at_; }
    void SetDeletedAt(const Poco::UInt64 value);

    Poco::UInt64 UpdatedAt() const { return updated_at_; }
    void SetUpdatedAt(const Poco::UInt64 value);

    std::string UpdatedAtString() const;
    void SetUpdatedAtString(const std::string value);

    // When a model is deleted
    // on server, it will be removed from local
    // DB using this flag:
    bool IsMarkedAsDeletedOnServer() const {
        return is_marked_as_deleted_on_server_;
    }
    void MarkAsDeletedOnServer() {
        is_marked_as_deleted_on_server_ = true;
        dirty_ = true;
    }

    virtual std::string String() const = 0;

    bool NeedsPush() const;
    bool NeedsPOST() const;
    bool NeedsPUT() const;
    bool NeedsDELETE() const;

  private:
    Poco::Int64 local_id_;
    Poco::UInt64 id_;
    guid guid_;
    Poco::UInt64 ui_modified_at_;
    Poco::UInt64 uid_;
    bool dirty_;
    Poco::UInt64 deleted_at_;
    bool is_marked_as_deleted_on_server_;
    Poco::UInt64 updated_at_;
  };

}  // namespace kopsik

#endif  // SRC_BASE_MODEL_H_