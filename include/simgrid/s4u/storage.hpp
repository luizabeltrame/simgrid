/* Copyright (c) 2006-2015. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#ifndef INCLUDE_SIMGRID_S4U_STORAGE_HPP_
#define INCLUDE_SIMGRID_S4U_STORAGE_HPP_

#include <boost/unordered_map.hpp>
#include "simgrid/simix.h"

namespace simgrid {
namespace s4u {

class Storage {
private:
	Storage(std::string name, smx_storage_t inferior);
	virtual ~Storage();
public:
	/** Retrieve a Storage by its name. It must exist in the platform file */
	static Storage &byName(const char* name);
	const char *name();
	sg_size_t size_free();
	sg_size_t size_used();
	/** Retrieve the total amount of space of this storage element */
	sg_size_t size();

	/* TODO: missing API:
XBT_PUBLIC(xbt_dict_t) MSG_storage_get_properties(msg_storage_t storage);
XBT_PUBLIC(void) MSG_storage_set_property_value(msg_storage_t storage, const char *name, char *value,void_f_pvoid_t free_ctn);
XBT_PUBLIC(const char *)MSG_storage_get_property_value(msg_storage_t storage, const char *name);
XBT_PUBLIC(xbt_dynar_t) MSG_storages_as_dynar(void);
XBT_PUBLIC(xbt_dict_t) MSG_storage_get_content(msg_storage_t storage);
XBT_PUBLIC(msg_error_t) MSG_storage_file_move(msg_file_t fd, msg_host_t dest, char* mount, char* fullname);
XBT_PUBLIC(const char *) MSG_storage_get_host(msg_storage_t storage);
	 */
protected:
	smx_storage_t inferior();
private:
	static boost::unordered_map<std::string, Storage *> *storages;
	std::string p_name;
	smx_storage_t p_inferior;


public:
	void set_userdata(void *data) {p_userdata = data;}
	void *userdata() {return p_userdata;}
private:
	void *p_userdata = NULL;

};

} /* namespace s4u */
} /* namespace simgrid */

#endif /* INCLUDE_SIMGRID_S4U_STORAGE_HPP_ */