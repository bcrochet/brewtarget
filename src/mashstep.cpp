/*
 * mashstep.cpp is part of Brewtarget, and is Copyright Philip G. Lee
 * (rocketman768@gmail.com), 2009.
 *
 * Brewtarget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Brewtarget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <string>
#include <QVector>
#include "mashstep.h"
#include "brewtarget.h"
#include <QDomElement>
#include <QDomText>

QStringList MashStep::types = QStringList() << "Infusion" << "Temperature" << "Decoction";

bool operator<(MashStep &m1, MashStep &m2)
{
   return m1.name < m2.name;
}

bool operator==(MashStep &m1, MashStep &m2)
{
   return m1.name == m2.name;
}

void MashStep::toXml(QDomDocument& doc, QDomNode& parent)
{
   QDomElement mashStepNode;
   QDomElement tmpNode;
   QDomText tmpText;
   
   mashStepNode = doc.createElement("MASH_STEP");
   
   tmpNode = doc.createElement("NAME");
   tmpText = doc.createTextNode(name);
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("VERSION");
   tmpText = doc.createTextNode(text(version));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("TYPE");
   tmpText = doc.createTextNode(getTypeString());
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("INFUSE_AMOUNT");
   tmpText = doc.createTextNode(text(infuseAmount_l));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("STEP_TEMP");
   tmpText = doc.createTextNode(text(stepTemp_c));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("STEP_TIME");
   tmpText = doc.createTextNode(text(stepTime_min));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("RAMP_TIME");
   tmpText = doc.createTextNode(text(rampTime_min));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("END_TEMP");
   tmpText = doc.createTextNode(text(endTemp_c));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("INFUSE_TEMP");
   tmpText = doc.createTextNode(text(infuseTemp_c));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("DECOCTION_AMOUNT");
   tmpText = doc.createTextNode(text(decoctionAmount_l));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   parent.appendChild(mashStepNode);
}

//==============================CONSTRUCTORS====================================

void MashStep::setDefaults()
{
   name = "";
   type = TYPEINFUSION;
   infuseAmount_l = 0.0;
   infuseTemp_c = 0.0;
   stepTemp_c = 0.0;
   stepTime_min = 0.0;
   rampTime_min = 0.0;
   endTemp_c = 0.0;
   decoctionAmount_l = 0.0;
}

MashStep::MashStep()
{
   setDefaults();
}

MashStep::MashStep(const QDomNode& mashStepNode)
{
   fromNode(mashStepNode);
}

void MashStep::fromNode(const QDomNode& mashStepNode)
{
   QDomNode node, child;
   QDomText textNode;
   QString property, value;
   
   setDefaults();
   
   for( node = mashStepNode.firstChild(); ! node.isNull(); node = node.nextSibling() )
   {
      if( ! node.isElement() )
      {
         Brewtarget::log(Brewtarget::WARNING, QObject::tr("Node at line %1 is not an element.").arg(textNode.lineNumber()) );
         continue;
      }
      
      child = node.firstChild();
      if( child.isNull() || ! child.isText() )
         continue;
      
      property = node.nodeName();
      textNode = child.toText();
      value = textNode.nodeValue();
      
      if( property == "NAME" )
      {
         name = value;
      }
      else if( property == "VERSION" )
      {
         if( version != getInt(textNode) )
            Brewtarget::log(Brewtarget::ERROR, QObject::tr("YEAST says it is not version %1. Line %2").arg(version).arg(textNode.lineNumber()) );
      }
      else if( property == "TYPE" )
      {
         int ndx = types.indexOf(value);
         if( ndx < 0 )
            Brewtarget::log(Brewtarget::ERROR, QObject::tr("%1 is not a valid type for MASHSTEP. Line %2").arg(value).arg(textNode.lineNumber()) );
         else
            setType(static_cast<MashStep::Type>(ndx));
      }
      else if( property == "INFUSE_AMOUNT" )
      {
         setInfuseAmount_l(getDouble(textNode));
      }
      else if( property == "STEP_TEMP" )
      {
         setStepTemp_c(getDouble(textNode));
      }
      else if( property == "STEP_TIME" )
      {
         setStepTime_min(getDouble(textNode));
      }
      else if( property == "RAMP_TIME" )
      {
         setRampTime_min(getDouble(textNode));
      }
      else if( property == "END_TEMP" )
      {
         setEndTemp_c(getDouble(textNode));
      }
      else if( property == "INFUSE_TEMP" )
      {
         setInfuseTemp_c(getDouble(textNode));
      }
      else if( property == "DECOCTION_AMOUNT" )
      {
         setDecoctionAmount_l(getDouble(textNode));
      }
      else
         Brewtarget::log(Brewtarget::WARNING, QObject::tr("Unsupported MASHSTEP property: %1. Line %2").arg(property).arg(node.lineNumber()) );
   }
}

//================================"SET" METHODS=================================
void MashStep::setName( const QString &var )
{
   name = QString(var);
   hasChanged();
}

void MashStep::setInfuseTemp_c(double var)
{
   infuseTemp_c = var;
   hasChanged();
}

void MashStep::setType( Type t )
{
   type = t;
   hasChanged();
}

void MashStep::setInfuseAmount_l( double var )
{
   if( var < 0.0 )
   {
      Brewtarget::logW( QString("Mashstep: number cannot be negative: %1").arg(var) );
      infuseAmount_l = 0;
      hasChanged();
   }
   else
   {
      infuseAmount_l = var;
      hasChanged();
   }
}

void MashStep::setStepTemp_c( double var )
{
   if( var < -273.15 )
   {
      Brewtarget::logW( QString("Mashstep: temp below absolute zero: %1").arg(var) );
      stepTemp_c = 0;
      hasChanged();
   }
   else
   {
      stepTemp_c = var;
      hasChanged();
   }
}

void MashStep::setStepTime_min( double var )
{
   if( var < 0.0 )
   {
      Brewtarget::logW( QString("Mashstep: step time cannot be negative: %1").arg(var) );
      stepTime_min = 0;
      hasChanged();
   }
   else
   {
      stepTime_min = var;
      hasChanged();
   }
}

void MashStep::setRampTime_min( double var )
{
   if( var < 0.0 )
   {
      Brewtarget::logW( QString("Mashstep: ramp time cannot be negative: %1").arg(var) );
      rampTime_min = 0;
      hasChanged();
   }
   else
   {
      rampTime_min = var;
      hasChanged();
   }
}

void MashStep::setEndTemp_c( double var )
{
   if( var < -273.15 )
   {
      Brewtarget::logW( QString("Mashstep: temp below absolute zero: %1").arg(var) );
      endTemp_c = 0;
      hasChanged();
   }
   else
   {
      endTemp_c = var;
      hasChanged();
   }
}

void MashStep::setDecoctionAmount_l(double var)
{
   decoctionAmount_l = var;
   hasChanged();
}

//============================="GET" METHODS====================================
QString MashStep::getName() const
{
   return name;
}

double MashStep::getInfuseTemp_c() const
{
   return infuseTemp_c;
}

MashStep::Type MashStep::getType() const
{
   return type;
}

const QString& MashStep::getTypeString() const
{
   return types.at(type);
}

const QString MashStep::getTypeStringTr() const
{
   QStringList typesTr = QStringList() << QObject::tr("Infusion") << QObject::tr("Temperature") << QObject::tr("Decoction");
   return typesTr.at(type);
}

double MashStep::getInfuseAmount_l() const
{
   return infuseAmount_l;
}

double MashStep::getStepTemp_c() const
{
   return stepTemp_c;
}

double MashStep::getStepTime_min() const
{
   return stepTime_min;
}

double MashStep::getRampTime_min() const
{
   return rampTime_min;
}

double MashStep::getEndTemp_c() const
{
   return endTemp_c;
}

double MashStep::getDecoctionAmount_l() const
{
   return decoctionAmount_l;
}

bool MashStep::isValidType( const QString &str ) const
{
   static const QString types[] = {"Infusion", "Temperature", "Decoction"};
   static const unsigned int size = 3;
   unsigned int i;
   
   for( i = 0; i < size; ++i )
      if( str == types[i] )
         return true;
   
   return false;
}
