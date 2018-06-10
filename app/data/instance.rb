add_include "app/models/machine.hpp", true
add_include "app/models/build.hpp", true
add_include "app/models/instance.hpp"

Model.add "Instance", ["Instance", "app/models/instance.hpp"] do
  resource_name "instance"
  order_by 'name'

  visibility :protected
  property 'std::string', 'name'
  property 'std::string', 'path'
  property 'std::string', 'user'
  property 'std::string', 'variables'
  property 'unsigned short', 'state', default: 0
  property 'std::string', 'last_configure', db: { column: 'last_conf' }
  property 'std::time_t', 'last_start'
  has_one "Machine", "machine", joined: false, dependent: :destroy
  has_one "Build",   "build",   joined: false, dependent: :destroy
end
