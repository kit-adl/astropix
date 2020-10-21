/*
 * ATLASPix3_SoftAndFirmware
 * Copyright (C) 2019  Rudolf Schimassek (rudolf.schimassek@kit.edu)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * This class is part of the project but was written by Thomas Fussell.
 * The code is available on GitHub: https://github.com/tfussell/miniz-cpp
 */

#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

struct mz_zip_archive_tag;

struct zip_info
{
    zip_info();
    std::string filename;
    struct
    {
        int year;
        int month;
        int day;
        int hours;
        int minutes;
        int seconds;
    } date_time;
    std::string comment;
    std::string extra;
    uint16_t create_system;
    uint16_t create_version;
    uint16_t extract_version;
    uint16_t flag_bits;
    std::size_t volume;
    uint32_t internal_attr;
    uint32_t external_attr;
    std::size_t header_offset;
    uint32_t crc;
    std::size_t compress_size;
    std::size_t file_size;
};

class zip_file
{
public:
    zip_file();
    zip_file(const std::string &filename);
    zip_file(const std::vector<unsigned char> &bytes);
    zip_file(std::istream &stream);
    ~zip_file();
    
    // to/from file
    void load(const std::string &filename);
    void save(const std::string &filename);
    
    // to/from byte vector
    void load(const std::vector<unsigned char> &bytes);
    void save(std::vector<unsigned char> &bytes);
    
    // to/from iostream
    void load(std::istream &stream);
    void save(std::ostream &stream);
    
    void reset();

    bool has_file(const std::string &name);
    bool has_file(const zip_info &name);
    
    zip_info getinfo(const std::string &name);
    
    std::vector<zip_info> infolist();
    std::vector<std::string> namelist();    //returns a list of the file names in the archive

    std::ostream &open(const std::string &name);
    std::ostream &open(const zip_info &name);
    
    void extract(const std::string &name);
    void extract(const std::string &name, const std::string &path);
    void extract(const zip_info &name);
    void extract(const zip_info &name, const std::string &path);
    
    void extractall();
    void extractall(const std::string &path);
    void extractall(const std::string &path, const std::vector<std::string> &members);
    void extractall(const std::string &path, const std::vector<zip_info> &members);
    
    //lists all files in the archive
    void printdir();
    void printdir(std::ostream &stream);
    
    std::string read(const std::string &name);
    std::string read(const zip_info &name);
    
    std::pair<bool, std::string> testzip();
    
    void write(const std::string &filename);
    void write(const std::string &filename, const std::string &arcname);
    
    void writestr(const std::string &arcname, const std::string &bytes);
    void writestr(const zip_info &arcname, const std::string &bytes);
    
    std::string get_filename() const { return filename_; }
    
    std::string comment;
    
private:
    void start_read();
    void start_write();
    
    void append_comment();
    void remove_comment();

    zip_info getinfo(int index);

    std::unique_ptr<mz_zip_archive_tag> archive_;
    std::vector<char> buffer_;
    std::stringstream open_stream_;
    std::string filename_;
};
