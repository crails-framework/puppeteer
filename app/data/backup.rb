add_include "app/models/instance.hpp", true
add_include "app/models/backup.hpp"

Model.add "Backup", ["Backup", "app/models/backup.hpp"] do
  resource_name "backup"

  visibility :protected
  property 'std::string',    'schedule',    default: "0 2 * * *"
  property 'unsigned short', 'max_history', default: 0

  has_one "Instance", "instance", joined: false, dependent: :destroy
end
