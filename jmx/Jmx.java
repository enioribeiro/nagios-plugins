import javax.management.MBeanServerConnection;
import javax.management.remote.JMXConnector;
import javax.management.remote.JMXConnectorFactory;
import javax.management.remote.JMXServiceURL;
import java.util.HashMap;
import javax.management.ObjectName;

public class Jmx{

	public static void main(String[] args) throws Exception {


		CliArgs cliArgs = new CliArgs(args);
		String  host = cliArgs.switchValue("-host");
		String  port = cliArgs.switchValue("-port");
		String  path = cliArgs.switchValue("-path");
		String  attr = cliArgs.switchValue("-attr");

		if(path == null || path ==""){
			path  = "jboss.as:deployment=pje-comum-api.war,subsystem=undertow"; 
		}

		if(attr == null || attr == ""){
			attr = "activeSessions"; 
		}


		HashMap   environment = new HashMap();
		String[]  credentials = new String[] {"jbossadmin", "jboss@123"};
	        environment.put(JMXConnector.CREDENTIALS, credentials);

		JMXServiceURL url = new JMXServiceURL("service:jmx:remote+http://" + host + ":" + port);
		JMXConnector jmxConn = JMXConnectorFactory.connect(url, environment);

		MBeanServerConnection mbsConn = jmxConn.getMBeanServerConnection();

		ObjectName  resource = new ObjectName(path);

		try{
			System.out.println(mbsConn.getAttribute(resource, attr).toString());
		}catch(Exception ex){
			System.out.println("Ops!!!\n" + ex);
		}

		jmxConn.close();
	}
}


