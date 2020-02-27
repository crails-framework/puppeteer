#ifndef  CONFIG_ODB_HPP
# define CONFIG_ODB_HPP

// Include this file in your odb headers to enable schema versioning
# pragma db model version(1,14)

# define podb odb::pgsql
# pragma db map type("INTEGER\\[\\]") as("TEXT") to("(?)::INTEGER[]") from("(?)::TEXT")

#endif
