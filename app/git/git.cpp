#include "git.hpp"
#include <git2.h>
#include <sstream>

using namespace Git;
using namespace std;

GlobalState::GlobalState()  { git_libgit2_init(); }
GlobalState::~GlobalState() { git_libgit2_shutdown(); }

//
// Repository
//
Repository::~Repository()
{
  if (handle != nullptr)
    git_repository_free(handle);
}

void Repository::clone(const string& url, const string& path)
{
  int error = git_clone(&handle, url.c_str(), path.c_str(), nullptr);

  if (error < 0)
    throw Git::Exception(error);
}

void Repository::open(const string& path)
{
  int error = git_repository_open(&handle, path.c_str());

  if (error < 0)
    throw Git::Exception(error);
}

std::shared_ptr<Remote> Repository::find_remote(const string& name)
{
  return std::make_shared<Remote>(handle, name);
}

void Repository::checkout(const string& name, int strategy)
{
  git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
  Git::Object treeish;
  int error;

  opts.checkout_strategy = strategy;
  error = git_revparse_single(&(treeish.handle), handle, name.c_str());
  if (error < 0) throw Git::Exception(error);
  error = git_checkout_tree(handle, treeish.handle, &opts);
  if (error < 0) throw Git::Exception(error);
}

std::string Repository::get_tip_oid() const
{
  unsigned char out[20];
  char readable_out[64];
  std::string oid;

  git_reference_name_to_id((git_oid*)out, handle, "HEAD");
  git_oid_fmt(readable_out, (git_oid*)out);
  oid.assign(readable_out, 40);
  return oid;
}

//
// Object
//
Object::Object() {}

Object::~Object()
{
  if (handle)
    git_object_free(handle);
}

//
// Remote
//
Remote::Remote(git_repository* repository_handle, const string& name)
{
  int error = git_remote_lookup(&handle, repository_handle, name.c_str());

  if (error < 0)
    throw Git::Exception(error);
}

Remote::~Remote()
{
  if (handle != nullptr)
    git_remote_free(handle);
}

void Remote::pull()
{
  fetch("pull");
}

void Remote::fetch(const string& operation)
{
  int error = git_remote_fetch(handle, NULL, NULL, operation.c_str());

  if (error < 0)
    throw Git::Exception(error);
}

string Remote::get_url() const
{
  return git_remote_url(handle);
}

Exception::Exception(int code)
{
  const git_error* e = giterr_last();
  std::stringstream stream;

  stream << "git error: " << code << '/' << e->klass << ": " << e->message;
  message = stream.str();
}
