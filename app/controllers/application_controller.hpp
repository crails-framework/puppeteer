#ifndef  APPLICATION_CONTROLLER_HPP
# define APPLICATION_CONTROLLER_HPP

# include <crails/server.hpp>
# include <crails/odb/controller.hpp>

class OArchive;

class ApplicationController : public Odb::Controller
{
protected:
  typedef Crails::Server::HttpCodes ResponseStatus;

public:
  ApplicationController(Crails::Params& params) : Odb::Controller(params)
  {}

  bool must_protect_from_forgery(void) const { return false; }

  void respond_with(Crails::Server::HttpCode);

  void render(const std::string& view);
  void render(const OArchive& archive);

  void require_parameters(const std::vector<std::string>& parameters) const;
};

#endif
