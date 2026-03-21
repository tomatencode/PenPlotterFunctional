#pragma once

#include <Arduino.h>
#include <vector>
#include <FS.h>

namespace storage {

bool fsInit();

std::vector<String> fsListFiles();

bool fsExists(const String& path);

bool fsDelete(const String& path);

File fsOpenRead(const String& path);

File fsOpenWrite(const String& path);

} // namespace storage