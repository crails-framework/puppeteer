add_include "app/models/machine.hpp", true
add_include "app/models/build.hpp", true
add_include "app/models/instance.hpp"

Model.add "Instance", ["Instance", "app/models/instance.hpp"] do
  resource_name "instance"

  visibility :protected
  property 'std::string', 'name'
  property 'std::string', 'path'
  property 'std::string', 'user'
  has_one "Machine", "machine", joined: false, dependent: :destroy
  has_one "Build",   "build",   joined: false, dependent: :destroy
end
