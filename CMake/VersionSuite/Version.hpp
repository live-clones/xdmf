/* Version Suite Class
 * Author: Brian Panneton
 */
#include <string>
#include <sstream>

/**
 * @brief Version Suite to assist in adding versioning to your project
 *
 * A simple way to have the library contain its own version.
 */
class Version {
	public:
		/**
                 * Create a Version class object
                 *
                 * @param name of the project
                 */
		Version(std::string cProjectName) { 
			setProjectName(cProjectName);
			setMajor(VersionMajor);
			setMinor(VersionMinor);
		}
	

		/**
		 * Get the version string
		 *
		 * @return the Version in "ProjectName Major.Minor" string format
		 */	
                std::string getVersion() {
			return 	getProjectName()+std::string(" ")+
				getMajorStr()+std::string(".")+
				getMinorStr();
                }

                /**
                 * Get the shorter version string
                 *
                 * @return the Version in "Major.Minor" string format
                 */  
		std::string getVersionShort() {
			return	getMajorStr()+std::string(".")+
				getMinorStr();
		}
	
		/**
                 * Get the version objects project name
                 *
                 * @return the project name in string format
                 */	
                std::string getProjectName() { return ProjectName; }
                
		/**
                 * Get the Version Major
                 *
                 * @return the Version Major in string format
                 */
		std::string getMajorStr() 
			{ return IntToStr(Major); }
		
		/**
                 * Get the Version Minor
                 *
                 * @return the Version Minor in string format
                 */
                std::string getMinorStr() 
			{ return IntToStr(Minor); }
		
		/**
                 * Get the Version Major
                 *
                 * @return the Version Major in int format
                 */
                int getMajor() { return Major; }
	
		/**
                 * Get the Version Minor
                 *
                 * @return the Version Minor in int format
                 */

                int getMinor() { return Minor; }

        private:
                std::string ProjectName;
		int Major, Minor;

		std::string IntToStr(int number) {
			std::stringstream s;
			s << number;
			return s.str();
		}

		void setProjectName(std::string iProjectName) 
			{ ProjectName = iProjectName; }
                void setMajor(int iMajor) { Major = iMajor; }
                void setMinor(int iMinor) { Minor = iMinor; }

};


