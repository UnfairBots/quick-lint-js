// Copyright (C) 2020  Matthew "strager" Glazar
// See end of file for extended copyright information.

#include <ostream>
#include <quick-lint-js/i18n/po-parser.h>
#include <quick-lint-js/port/char8.h>

namespace quick_lint_js {
std::ostream& operator<<(std::ostream& out, const PO_Entry& entry) {
  out << "PO_File{\n";
  out << "  .msgid = \"" << out_string8(entry.msgid) << "\",\n";
  out << "  .msgstr = \"" << out_string8(entry.msgstr) << "\",\n";
  out << "  .is_fuzzy = " << (entry.is_fuzzy ? "true" : "false") << ",\n";
  out << "}";
  return out;
}
}

// quick-lint-js finds bugs in JavaScript programs.
// Copyright (C) 2020  Matthew "strager" Glazar
//
// This file is part of quick-lint-js.
//
// quick-lint-js is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// quick-lint-js is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with quick-lint-js.  If not, see <https://www.gnu.org/licenses/>.