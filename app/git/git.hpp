#ifndef  GIT_HPP
# define GIT_HPP

# include <string>
# include <exception>
# include <memory>
# include <git2.h>

namespace Git
{
  struct GlobalState
  {
    GlobalState();
    ~GlobalState();
  };

  struct Object
  {
    Object();
    ~Object();

    git_object* handle = nullptr;
  };

  class Remote
  {
  public:
   Remote(git_repository*, const std::string&);
   ~Remote();

   void fetch(const std::string& operation = "fetch");
   void pull();

   std::string get_url() const;

  private:
    git_remote* handle = nullptr;
  };

  class Repository
  {
  public:
    ~Repository();

    void clone(const std::string& url, const std::string& path);
    void open(const std::string& path);

    std::shared_ptr<Remote> find_remote(const std::string& name);
    void checkout(const std::string& name, int strategy = GIT_CHECKOUT_SAFE);
    std::string get_tip_oid() const;

  private:
    git_repository* handle = nullptr;
  };

  struct Exception : public std::exception
  {
    Exception(int code);

    const char* what() const noexcept { return message.c_str(); }

  private:
    std::string message;
  };
}

#endif
