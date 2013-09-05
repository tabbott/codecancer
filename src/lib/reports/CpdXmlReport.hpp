#pragma once

#include "ReportBase.hpp"

#include <rapidxml.hpp>
#include <boost/lexical_cast.hpp>

#include <ostream>
#include <string>

class EsaView;
class LcpInterval;

class CpdXmlReport : public ReportBase {
public:
    CpdXmlReport(EsaView const& esaView, std::ostream& out);

    void observe(LcpInterval const& interval);
    void finalize();

private:
    EsaView const& _esaView;
    std::ostream& _out;
    rapidxml::xml_document<> _doc;
    rapidxml::xml_node<>* _docRoot;

private:
    char* xmlString(std::string const& value) {
        return _doc.allocate_string(value.data(), value.size());
    }

    template<typename T>
    char* xmlString(T const& value) {
        std::string valueStr = boost::lexical_cast<std::string>(value);
        return xmlString(valueStr);
    }

    template<typename Node, typename T>
    void add_attribute(Node* node, std::string const& name, T const& value) {
        auto nameStr = xmlString(name);
        auto valueStr = xmlString(value);
        auto attr = _doc.allocate_attribute(nameStr, valueStr);
        node->append_attribute(attr);
    }
};
